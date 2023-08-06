#ifndef __DEEP_ENGINE_CORE_HPP__
#define __DEEP_ENGINE_CORE_HPP__

#include <DE/def.h>
#include <DE/error.hpp>

#include <stdint.h>

namespace de {

	class DE_API Core {

		private:
			static uint64_t m_InitTime;

		public:
			static ErrorStatus init();

			static uint32_t getMousePosition(int *x, int *y);

			static void quit();

			static uint64_t getTick();
			static uint64_t getCurrentTimeMillis();
			static void sleep(uint32_t millis);

	};

	inline uint64_t Core::getTick() {
#ifdef DE_WINDOWS
		return GetTickCount64() - m_InitTime;
#else
#error Need implementation
#endif
	}

	inline void Core::sleep(uint32_t millis)
	{
#ifdef DE_WINDOWS
		Sleep(millis);
#else
#error Need implementation
#endif
	}

}

#endif