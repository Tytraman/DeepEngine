#include "DE/core.hpp"
#include "DE/memory/memory.hpp"
#include "DE/c-wrapper/core.h"
#include "DE/gui/deimgui.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/resources.hpp"
#include "DE/mutex.hpp"
#include "DE/string.hpp"
#include "DE/window.hpp"
#include "DE/memory/settings.hpp"
#include "DE/os/COM.hpp"
#include "DE/hardware/cpu.hpp"

#include <SDL.h>

namespace deep
{

    #ifdef DE_WINDOWS
    uint64_t deep::core::m_InitTime = GetTickCount64();
    #else
    #error Need implementation
    #endif

    /*
    ======================
    core::getMousePosition
    ======================
    */
    uint32_t core::getMousePosition(int *x, int *y)
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
        HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if(stdHandle != INVALID_HANDLE_VALUE)
        {
            DWORD consoleMode;
            GetConsoleMode(stdHandle, &consoleMode);
            consoleMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(stdHandle, consoleMode);
        }
#endif

        printf(DE_TERM_FG_GREEN "core::init" DE_TERM_RESET " " DE_VERSION " - " DE_TERM_FG_RED "this is where it all begins" DE_TERM_RESET "\n");

        // Vérifie s'il y a une autre instance du programme et
        // si c'est le cas, lui donne le focus.
        if(focusInstance(gameTitle))
            return core_init_status::InstanceAlreadyExists;

        // Vérifie qu'il y a suffisamment de place dans le disque.
        if(!checkAvailableDiskSpace(diskSpaceRequired))
            return core_init_status::NoEnoughDiskSpace;

        // Vérifie qu'il y a suffisamment de mémoire.
        if(!checkMemory(physicalRamNeeded, virtualRamNeeded))
            return core_init_status::NoEnoughMemory;

        deep::engine_settings *engineSettings = deep::engine_settings::get_singleton();

        // Charge les paramètres liés au moteur.
        if(!engineSettings->init("engine_config.fobj"))
            return core_init_status::CannotLoadEngineSettings;

        if(SDL_Init(SDL_INIT_VIDEO) != 0)
            return core_init_status::Unknown;

        COM *com = COM::get_singleton();
        if(!com->init())
            return core_init_status::CannotInitCOM;

        

        printf(DE_TERM_FG_YELLOW "====================[ " DE_TERM_FG_RED "CPU" DE_TERM_FG_YELLOW " ]====================\n" DE_TERM_RESET);

        CPU *cpu = CPU::get_singleton();
        if(!cpu->query_info())
            return core_init_status::CannotQueryCpuInfo;

        printf(
            DE_TERM_RESET "Name: %s\n"
            "Architecture: %s\n"
            "Address width: %u-bit\n"
            DE_TERM_FG_YELLOW "===============================================" DE_TERM_RESET "\n", cpu->get_name().str(), cpu->get_architecture_str(), cpu->get_address_width());

        printf(DE_TERM_FG_GREEN "core::init'ialisation successful" DE_TERM_RESET "\n");

        return core_init_status::OK;
    }

    /*
    =============================
    core::checkAvailableDiskSpace
    =============================
    */
    bool core::checkAvailableDiskSpace(uint64_t diskSpaceRequired)
    {
        printf("Checking available disk space... ");
        fflush(stdout);

#if DE_WINDOWS
        ULARGE_INTEGER space;

        if(!GetDiskFreeSpaceExA(nullptr, &space, nullptr, nullptr))
        {
            printf("BAD\n");
            return false;
        }

        if(space.QuadPart >= diskSpaceRequired)
        {
            printf("OK\n");
            return true;
        }

        printf("BAD\n");
        return false;
#else
#error Need implementation
#endif
    }

    /*
    =================
    core::checkMemory
    =================
    */
    bool core::checkMemory(uint64_t physicalRamNeeded, uint64_t virtualRamNeeded)
    {
        printf("Checking memory... ");
        fflush(stdout);

#if DE_WINDOWS

        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);

        if(!GlobalMemoryStatusEx(&status))
        {
            printf("BAD\n");
            return false;
        }

        if(physicalRamNeeded > status.ullTotalPhys || virtualRamNeeded > status.ullAvailVirtual)
        {
            printf("BAD\n");
            return false;
        }

        // Vérifie qu'il est possible de réserver une plage continue d'octets.
        if(virtualRamNeeded > 0)
        {
            mem_ptr ptr = mem::alloc_no_track(virtualRamNeeded);
            if(ptr == nullptr)
            {
                printf("BAD\n");
                return false;
            }

            mem::free_no_track(ptr);
        }
        
#else
#error Need implementation
#endif

        printf("OK\n");
        return true;
    }

    /*
    ====================
    core::isOnlyInstance
    ====================
    */
    bool core::focusInstance(const char *gameTitle)
    {
        const char *retText = "OK";

        printf("Checking if another instance of the game is not currently running... ");
        fflush(stdout);

        mutex_handle mutexHandle = mutex::create(gameTitle);
        bool ret = false;

        if(mutexHandle == invalid_mutex_handle)
        {
            retText = "BAD";
            ret = true;

            goto end;
        }

        window_handle win = window::find(gameTitle);
        if(win != invalid_window_handle)
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
        printf("%s\n", retText);
        return ret;
    }

    /*
    ==============
    core::shutdown
    ==============
    */
    void core::shutdown()
    {
        COM *com = COM::get_singleton();
        com->shutdown();

        im_gui_window::shutdown();
        scene::shutdown();
        SDL_Quit();
    }

    /*
    ==========================
    core::getCurrentTimeMillis
    ==========================
    */
    uint64_t core::getCurrentTimeMillis()
    {
        FILETIME t;
        uint64_t millis;
        GetSystemTimeAsFileTime(&t);
        millis = ((uint64_t) t.dwLowDateTime + ((uint64_t) (t.dwHighDateTime) << 32)) / 10000;

        return millis;
    }

    /*
    ==================
    core::getLocalTime
    ==================
    */
    void core::getLocalTime(uint32_t *year, uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, uint32_t *millis)
    {
#if DE_WINDOWS
        SYSTEMTIME st;
        GetLocalTime(&st);

        if(year != nullptr)
            *year = st.wYear;

        if(month != nullptr)
            *month = st.wMonth;

        if(day != nullptr)
            *day = st.wDay;

        if(hour != nullptr)
            *hour = st.wHour;

        if(minute != nullptr)
            *minute = st.wMinute;

        if(second != nullptr)
            *second = st.wSecond;

        if(millis != nullptr)
            *millis = st.wMilliseconds;
#else
#error Need implementation.
#endif
    }

    /*
    ============
    core::getPwd
    ============
    */
    const char *core::getPwd()
    {
        static string p;

#if DE_WINDOWS
        DWORD pwdLength = GetCurrentDirectoryA(0, NULL);
        char *buffer = (char *) mem::alloc_no_track(pwdLength);
        GetCurrentDirectoryA(pwdLength, buffer);
        p = buffer;
#else
#error Need implementation.
#endif

        return p.str();
    }

}

/*
================
de_core_get_tick
================
*/
de_uint64 de_core_get_tick() {
    return deep::core::getTick();
}
