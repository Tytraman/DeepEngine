#include <DE/core.hpp>
#include <DE/c-wrapper/core.h>

#include <SDL.h>

#ifdef DE_WINDOWS
	uint64_t de::Core::_initTime = GetTickCount64();
#else
#error Need implementation
#endif

de::ErrorStatus de::Core::init() {
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		return ErrorStatus::InitSDL;

	return ErrorStatus::NoError;
}

de_uint64 de_core_get_tick() {
	return de::Core::getTick();
}

void de::Core::quit() {
	SDL_Quit();
}
