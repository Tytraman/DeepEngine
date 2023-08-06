#include <DE/core.hpp>
#include <DE/c-wrapper/core.h>

#include <SDL.h>

namespace de {

	#ifdef DE_WINDOWS
	uint64_t de::Core::m_InitTime = GetTickCount64();
	#else
	#error Need implementation
	#endif

	uint32_t Core::getMousePosition(int *x, int *y)
	{
		return SDL_GetMouseState(x, y);
	}

	ErrorStatus Core::init()
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			return ErrorStatus::InitSDL;

		return ErrorStatus::NoError;
	}

	void Core::quit() {
		SDL_Quit();
	}

	uint64_t Core::getCurrentTimeMillis()
	{
		FILETIME t;
		uint64_t millis;
		GetSystemTimeAsFileTime(&t);
		millis = ((uint64_t) t.dwLowDateTime + ((uint64_t) (t.dwHighDateTime) << 32)) / 10000;

		return millis;
	}

}

de_uint64 de_core_get_tick() {
	return de::Core::getTick();
}
