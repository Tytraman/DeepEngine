#ifndef __DEEP_ENGINE_STRING_UTILS_H__
#define __DEEP_ENGINE_STRING_UTILS_H__

#include <DE/def.hpp>

#include <stddef.h>

// ======= PURE C ======= //
// | Un simple wrapper. | //
// ====================== //

DE_BEGIN_EXTERN_C

	DE_API size_t de_string_utils_copy(char *dest, size_t destSize, const char *source, size_t sourceLength);
	DE_API int de_string_utils_to_upper(int value);

DE_END_EXTERN_C

#endif