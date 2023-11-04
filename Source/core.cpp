#include <DE/core.hpp>
#include <DE/memory/memory.hpp>
#include <DE/c-wrapper/core.h>
#include <DE/imgui/deimgui.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/resources.hpp>

#include <string>

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

	error_status core::init()
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			return error_status::InitSDL;

		return error_status::NoError;
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
		static std::string p;

#if DE_WINDOWS
		DWORD pwdLength = GetCurrentDirectoryA(0, NULL);
		char *buffer = (char *) mem::allocNoTrack(pwdLength);
		GetCurrentDirectoryA(pwdLength, buffer);
		p.assign(buffer);
#else
#error Need implementation.
#endif

		return p.c_str();
	}

}

de_uint64 de_core_get_tick() {
	return de::core::getTick();
}
