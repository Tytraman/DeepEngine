#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>

#include <DE/c-wrapper/string_utils.h>

#include <string.h>

namespace de {

	size_t StringUtils::copy(char *dest, size_t destSize, const char *source, size_t sourceLength) {
		size_t len = 0;
	
		if(destSize == 0)
			return 0;

		while(len < sourceLength) {
			if(len == destSize - 1)
				break;
			dest[len] = source[len];
			len++;
		}

		dest[len] = '\0';

		return len;
	}

	char *StringUtils::copy(const char *source)
	{
		size_t len = strlen(source);
		char *dest = (char *) mem::alloc(len + 1);

		if(dest == nullptr)
			return nullptr;

		memcpy(dest, source, len);
		dest[len] = '\0';

		return dest;
	}

	int StringUtils::toUpper(int value) {
		if(value >= 'a' && value <= 'z')
			return value - 32;
		return value;
	}
}

size_t de_string_utils_copy(char *dest, size_t destSize, const char *source, size_t sourceLength) {
	return de::StringUtils::copy(dest, destSize, source, sourceLength);
}

int de_string_utils_to_upper(int value) {
	return de::StringUtils::toUpper(value);
}
