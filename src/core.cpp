#include <DE/core.hpp>
#include <DE/memory/memory.hpp>
#include <DE/c-wrapper/core.h>
#include <DE/imgui/deimgui.hpp>
#include <DE/ecs/scene.hpp>

#include <string>

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

	void Core::shutdown()
	{
		ImGuiWindow::shutdown();
		Scene::shutdown();
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

	const char *Core::getPwd()
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
	return de::Core::getTick();
}
