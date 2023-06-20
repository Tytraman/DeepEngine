#ifndef __DEEP_ENGINE_STRING_UTILS_HPP__
#define __DEEP_ENGINE_STRING_UTILS_HPP__

#include <DE/def.h>
#include <stddef.h>

namespace de {

	class DE_API StringUtils {

		public:
			static size_t copy(char *dest, size_t destSize, const char *source, size_t sourceLength);
			static int toUpper(int value);

	};

}

#endif