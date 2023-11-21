#include <DE/core.hpp>
#include <DE/memory/memory.hpp>
#include <DE/c-wrapper/core.h>
#include <DE/imgui/deimgui.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/resources.hpp>
#include <DE/mutex.hpp>
#include <DE/string.hpp>
#include <DE/window.hpp>

#include <SDL.h>

namespace de
{

	#ifdef DE_WINDOWS
	uint64_t de::core::m_InitTime = GetTickCount64();
	#else
	#error Need implementation
	#endif

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

		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			return core_init_status::Unknown;

		return core_init_status::OK;
	}

    /*
	=============================
	core::checkAvailableDiskSpace
	=============================
	*/
    bool core::checkAvailableDiskSpace(uint64_t diskSpaceRequired)
    {
#if DE_WINDOWS
        ULARGE_INTEGER space;

        if(!GetDiskFreeSpaceExA(nullptr, &space, nullptr, nullptr))
            return false;

        return space.QuadPart >= diskSpaceRequired;
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
#if DE_WINDOWS

        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);

        if(!GlobalMemoryStatusEx(&status))
            return false;

        if(physicalRamNeeded > status.ullTotalPhys || virtualRamNeeded > status.ullAvailVirtual)
            return false;

        // Vérifie qu'il est possible de réserver une plage continue d'octets.
        if(virtualRamNeeded > 0)
        {
            mem_ptr ptr = mem::allocNoTrack(virtualRamNeeded);
            if(ptr == nullptr)
                return false;

            mem::freeNoTrack(ptr);
        }
        
#else
#error Need implementation
#endif

        return true;
    }

    /*
	====================
	core::isOnlyInstance
	====================
	*/
    bool core::focusInstance(const char *gameTitle)
    {
        mutex_handle mutexHandle = mutex::create(gameTitle);
        bool ret = false;

        if(mutexHandle == invalid_mutex_handle)
            return true;

        window_handle win = window::find(gameTitle);
        if(win != invalid_window_handle)
        {
            window::show(win);
            window::focus(win);
            window::setForeground(win);
            window::setActive(win);

            ret = true;
        }

        mutex::close(mutexHandle);

        return ret;
    }

	void core::shutdown()
	{
		im_gui_window::shutdown();
		scene::shutdown();
		SDL_Quit();
	}

	uint64_t core::getCurrentTimeMillis()
	{
		FILETIME t;
		uint64_t millis;
		GetSystemTimeAsFileTime(&t);
		millis = ((uint64_t) t.dwLowDateTime + ((uint64_t) (t.dwHighDateTime) << 32)) / 10000;

		return millis;
	}

	const char *core::getPwd()
	{
		static string p;

#if DE_WINDOWS
		DWORD pwdLength = GetCurrentDirectoryA(0, NULL);
		char *buffer = (char *) mem::allocNoTrack(pwdLength);
		GetCurrentDirectoryA(pwdLength, buffer);
		p = buffer;
#else
#error Need implementation.
#endif

		return p.str();
	}

}

de_uint64 de_core_get_tick() {
	return de::core::getTick();
}
