#ifndef __DEEP_ENGINE_CORE_HPP__
#define __DEEP_ENGINE_CORE_HPP__

#include <DE/def.h>
#include <DE/error.hpp>

namespace de {

	class DE_API Core {

		private:
			static uint64 _initTime;

		public:
			static ErrorStatus init();

			static void quit();

			static uint64 getTick();

	};

	inline uint64 Core::getTick() {
#ifdef DE_WINDOWS
		return GetTickCount64() - _initTime;
#else
#error Need implementation
#endif
	}

}

#endif