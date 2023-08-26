#ifndef __DEEP_ENGINE_ERROR_HPP__
#define __DEEP_ENGINE_ERROR_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stdint.h>

#define DE_ERROR_SW_HELPER(__status,...) switch(__status) { __VA_ARGS__ }
#define DE_ERROR_CASE_HELPER(__name) case __name:{error = #__name; }break

#define DE_ERROR_SW(__status,...) DE_ERROR_SW_HELPER(__status,__VA_ARGS__)
#define DE_ERROR_CASE(__status) DE_ERROR_CASE_HELPER(__status)

namespace de {

	enum ErrorStatus : uint16_t {
		NoError,
		InitSDL,
		CreateWindowSDL,
		CreateRendererSDL,
		NoWindowFound,
		GLCreateContext
	};

	class DE_API DError {

		public:
			static const char *Error;
			static const char *Warning;
			static const char *Success;

			static void printError(ErrorStatus status);
	};

}

#endif