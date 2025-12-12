#include "core/core.hpp"
#include "core/memory.hpp"
#include "gui/deimgui.hpp"
#include "ecs/scene.hpp"
#include "core/resources/resource_manager.hpp"
#include "os/mutex.hpp"
#include "core/string/string.hpp"
#include "os/window.hpp"
#include "core/config/settings.hpp"
#include "os/COM.hpp"
#include "hardware/cpu.hpp"
#include "io/file_stream.hpp"
#include "io/stream_writer.hpp"
#include "core/templates/list.hpp"
#include "os/win32.hpp"

#include <SDL2/SDL.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace deep
{

#ifdef DE_WINDOWS
    uint64_t core::m_InitTime = GetTickCount64();
#else
#error Need implementation
#endif

    ref<text_writer> core::m_Stderr;
    ref<text_writer> core::m_Stdout;

    /*
    ========================
    core::get_mouse_position
    ========================
    */
    uint32_t core::get_mouse_position(int *x, int *y)
    {
        return SDL_GetMouseState(x, y);
    }

    /*
    ==========
    core::init
    ==========
    */
    core_init_status core::init(const char *gameTitle, uint64_t diskSpaceRequired, uint64_t physicalRamNeeded, uint64_t virtualRamNeeded)
    {
#if DE_WINDOWS
        HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE stderrHandle = GetStdHandle(STD_ERROR_HANDLE);

        if (stdoutHandle != INVALID_HANDLE_VALUE)
        {
            DWORD consoleMode;
            GetConsoleMode(stdoutHandle, &consoleMode);
            consoleMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(stdoutHandle, consoleMode);
        }

        if (stderrHandle != INVALID_HANDLE_VALUE)
        {
            DWORD consoleMode;
            GetConsoleMode(stderrHandle, &consoleMode);
            consoleMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(stderrHandle, consoleMode);
        }

        // TODO: ne pas allouer les singletons ici.
        // Allocation des singletons.

        list<mem_ptr> *memoryTrack = mem::alloc_type_no_track<list<mem_ptr>>();
        if (memoryTrack == nullptr)
        {
            return core_init_status::CannotInstantiateObjects;
        }

        mem::set_memory_track(memoryTrack);

        CPU *ccpu = mem::alloc_type_no_track<CPU>();
        if (ccpu == nullptr)
        {
            mem::free_type_no_track(memoryTrack);


            return core_init_status::CannotInstantiateObjects;
        }

        CPU::set_singleton(ccpu);

        engine_settings *sset = mem::alloc_type_no_track<engine_settings>();
        if (sset == nullptr)
        {
            mem::free_type_no_track(ccpu);
            mem::free_type_no_track(memoryTrack);

            return core_init_status::CannotInstantiateObjects;
        }

        engine_settings::set_singleton(sset);

        HMODULE hModule = GetModuleHandleA("NTDLL");
        if (hModule == nullptr)
        {
            return core_init_status::CannotInitNtDll;
        }

        nt_query_object = (NtQueryObject) GetProcAddress(hModule, "NtQueryObject");
        if (nt_query_object == nullptr)
        {
            return core_init_status::CannotInitNtDll;
        }

        ref<file_stream> stdoutStream = mem::alloc_type<file_stream>("stdout", stdoutHandle);
        ref<stream_writer> stdoutWriter = mem::alloc_type<stream_writer>(stdoutStream.get());

        ref<file_stream> stderrStream = mem::alloc_type<file_stream>("stderr", stderrHandle);
        ref<stream_writer> stderrWriter = mem::alloc_type<stream_writer>(stderrStream.get());

        if (!stdoutWriter->open())
        {
            return core_init_status::CannotLoadStdStream;
        }

        if (!stderrWriter->open())
        {
            return core_init_status::CannotLoadStdStream;
        }

        m_Stdout = stdoutWriter.get();
        m_Stderr = stderrWriter.get();
#endif

        *m_Stdout << DE_TERM_FG_GREEN "core::init" DE_TERM_RESET " " DE_VERSION " - " DE_TERM_FG_RED "this is where it all begins" DE_TERM_RESET "\n";

        // Vérifie s'il y a une autre instance du programme et
        // si c'est le cas, lui donne le focus.
        if (focus_instance(gameTitle))
        {
            return core_init_status::InstanceAlreadyExists;
        }

        // Vérifie qu'il y a suffisamment de place dans le disque.
        if (!check_available_disk_space(diskSpaceRequired))
        {
            return core_init_status::NoEnoughDiskSpace;
        }

        // Vérifie qu'il y a suffisamment de mémoire.
        if (!check_memory(physicalRamNeeded, virtualRamNeeded))
        {
            return core_init_status::NoEnoughMemory;
        }

        engine_settings *engineSettings = engine_settings::get_singleton();

        ref<file_stream> ifs(mem::alloc_type<file_stream>("engine_settings.fobj", file_stream::file_mode::Open, file_stream::file_access::Read, file_stream::file_share::Read));

        // Charge les paramètres liés au moteur.
        if (!engineSettings->init(ifs.get()))
        {
            return core_init_status::CannotLoadEngineSettings;
        }

        ref<file_stream> resFs = mem::alloc_type<file_stream>("resources.deep", file_stream::file_mode::OpenOrCreate, file_stream::file_access::ReadWrite, file_stream::file_share::Read);

        resource_manager *resManager = resource_manager::get_singleton();
        if (!resManager->init(resFs.get()))
        {
            return core_init_status::CannotInitResources;
        }

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            return core_init_status::Unknown;
        }

        COM *com = COM::get_singleton();
        if (!com->init())
        {
            return core_init_status::CannotInitCOM;
        }

        *m_Stdout << DE_TERM_FG_YELLOW "====================[ " DE_TERM_FG_RED "CPU" DE_TERM_FG_YELLOW " ]====================\n" DE_TERM_RESET;

        CPU *cpu = CPU::get_singleton();
        if (!cpu->query_info())
        {
            return core_init_status::CannotQueryCpuInfo;
        }

        *m_Stdout <<
            DE_TERM_RESET "Name: " << cpu->get_name().str() << "\n"
            "Architecture: " << cpu->get_architecture_str() << "\n"
            "Address width: " << cpu->get_address_width() << "-bit\n"
            DE_TERM_FG_YELLOW "===============================================" DE_TERM_RESET "\n";

        *m_Stdout << DE_TERM_FG_GREEN "core::init'ialisation successful" DE_TERM_RESET "\n";

        return core_init_status::OK;
    }

    /*
    ================================
    core::check_available_disk_space
    ================================
    */
    bool core::check_available_disk_space(uint64_t diskSpaceRequired)
    {
        *m_Stdout << "Checking available disk space... ";

#if DE_WINDOWS
        ULARGE_INTEGER space;

        if (!GetDiskFreeSpaceExA(nullptr, &space, nullptr, nullptr))
        {
            *m_Stdout << "BAD\n";
            return false;
        }

        if (space.QuadPart >= diskSpaceRequired)
        {
            *m_Stdout << "OK\n";
            return true;
        }

        *m_Stdout << "BAD\n";
        return false;
#else
#error Need implementation
#endif
    }

    /*
    ==================
    core::check_memory
    ==================
    */
    bool core::check_memory(uint64_t physicalRamNeeded, uint64_t virtualRamNeeded)
    {
        *m_Stdout << "Checking memory... ";

#if DE_WINDOWS

        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);

        if (!GlobalMemoryStatusEx(&status))
        {
            *m_Stdout << "BAD\n";
            return false;
        }

        if (physicalRamNeeded > status.ullTotalPhys || virtualRamNeeded > status.ullAvailVirtual)
        {
            *m_Stdout << "BAD\n";
            return false;
        }

        // Vérifie qu'il est possible de réserver une plage continue d'octets.
        if (virtualRamNeeded > 0)
        {
            mem_ptr ptr = mem::alloc_no_track(virtualRamNeeded);
            if (ptr == nullptr)
            {
                *m_Stdout << "BAD\n";
                return false;
            }

            mem::free_no_track(ptr);
        }

#else
#error Need implementation
#endif

        *m_Stdout << "OK\n";
        return true;
    }

    /*
    ====================
    core::focus_instance
    ====================
    */
    bool core::focus_instance(const char *gameTitle)
    {
        const char *retText = "OK";

        *m_Stdout << "Checking if another instance of the game is not currently running... ";

        mutex_handle mutexHandle = mutex::create(gameTitle);
        bool ret = false;

        if (mutexHandle == invalid_mutex_handle)
        {
            retText = "BAD";
            ret = true;

            goto end;
        }

        window_handle win = window::find(gameTitle);
        if (win != invalid_window_handle)
        {
            window::show(win);
            window::focus(win);
            window::set_foreground(win);
            window::set_active(win);

            retText = "BAD";
            ret = true;
        }

        mutex::close(mutexHandle);

end:
        *m_Stdout << retText << "\n";

        return ret;
    }

    /*
    ==============
    core::shutdown
    ==============
    */
    void core::shutdown()
    {
        core::out() << DE_TERM_RESET DE_TERM_FG_GREEN "core::shutdown" DE_TERM_FG_RED " this is where it all ends" DE_TERM_RESET "\n";

        COM *com = COM::get_singleton();
        com->shutdown();

        im_gui_window::shutdown();
        scene::shutdown();

        mem::free_type_no_track(engine_settings::get_singleton());
        mem::free_type_no_track(CPU::get_singleton());
        mem::free_type_no_track(resource_manager::get_singleton());
        mem::free_type_no_track(mem::get_memory_track());

        engine_settings::set_singleton(nullptr);
        CPU::set_singleton(nullptr);
        mem::set_memory_track(nullptr);

        SDL_Quit();

        core::out() << DE_TERM_RESET DE_TERM_FG_RED "~Good-bye~" DE_TERM_RESET "\n";
    }

    /*
    =============
    core::set_out
    =============
    */
    void core::set_out(text_writer *writer)
    {
        m_Stdout = writer;
    }

    /*
    =============================
    core::get_current_time_millis
    =============================
    */
    uint64_t core::get_current_time_millis()
    {
        FILETIME t;
        uint64_t millis;
        GetSystemTimeAsFileTime(&t);
        millis = ((uint64_t) t.dwLowDateTime + ((uint64_t) (t.dwHighDateTime) << 32)) / 10000;

        return millis;
    }

    /*
    ====================
    core::get_local_time
    ====================
    */
    void core::get_local_time(uint32_t *year, uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, uint32_t *millis)
    {
#if DE_WINDOWS
        SYSTEMTIME st;
        GetLocalTime(&st);

        if (year != nullptr)
            *year = st.wYear;

        if (month != nullptr)
            *month = st.wMonth;

        if (day != nullptr)
            *day = st.wDay;

        if (hour != nullptr)
            *hour = st.wHour;

        if (minute != nullptr)
            *minute = st.wMinute;

        if (second != nullptr)
            *second = st.wSecond;

        if (millis != nullptr)
            *millis = st.wMilliseconds;
#else
#error Need implementation.
#endif
    }

    /*
    =============
    core::set_pwd
    =============
    */
    bool core::set_pwd(const char *path)
    {
#if DE_WINDOWS

        return SetCurrentDirectoryA(path);

#else
#error Need implementation.
#endif
    }

    /*
    =============
    core::get_pwd
    =============
    */
    string core::get_pwd()
    {
#if DE_WINDOWS
        DWORD pwdLength = GetCurrentDirectoryA(0, NULL);
        char *buffer = (char *) mem::alloc_no_track(pwdLength);
        GetCurrentDirectoryA(pwdLength, buffer);
        string p = buffer;

        return p;
#else
#error Need implementation.
#endif


    }

}
