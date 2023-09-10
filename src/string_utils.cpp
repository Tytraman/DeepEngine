#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>

#include <DE/c-wrapper/string_utils.h>

#include <string.h>
#include <stdio.h>

namespace de {

	/*
	=================
	StringUtils::copy
	=================
	*/
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

	/*
	=================
	StringUtils::copy
	=================
	*/
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

	/*
	=================
	StringUtils::copy
	=================
	*/
	wchar_t *StringUtils::copy(const wchar_t *source)
	{
		size_t len = length(source);
		wchar_t *dest = (wchar_t *) mem::alloc((len * sizeof(wchar_t)) + sizeof(wchar_t));

		if(dest == nullptr)
			return nullptr;

		memcpy(dest, source, len * sizeof(wchar_t));
		dest[len] = L'\0';

		return dest;
	}

	/*
	====================
	StringUtils::toUpper
	====================
	*/
	int StringUtils::toUpper(int value) {
		if(value >= 'a' && value <= 'z')
			return value - 32;
		return value;
	}

	/*
	===================
	StringUtils::length
	===================
	*/
	size_t StringUtils::length(const wchar_t *str)
	{
		size_t len = 0;

		while(str[len] != L'\0')
			len++;

		return len;
	}

	/*
	===================
	StringUtils::append
	===================
	*/
	bool StringUtils::append(wchar_t **dest, const wchar_t *source)
	{
		size_t destLen = length(*dest);
		size_t sourceLen = length(source);
		size_t len = destLen + sourceLen;

		mem_ptr ptr = mem::realloc(*dest, len * sizeof(wchar_t) + sizeof(wchar_t));
		if(ptr == nullptr)
			return false;

		*dest = (wchar_t *) ptr;

		memcpy(*dest + destLen, source, sourceLen * sizeof(wchar_t));

		*(*dest + len) = L'\0';

		return true;
	}

	/*
	=====================
	StringUtils::endsWith
	=====================
	*/
	bool StringUtils::endsWith(const wchar_t *toSearch, const wchar_t *end)
	{
		size_t len1 = length(toSearch);
		size_t len2 = length(end);

		// Si la chaîne de fin est plus longue que la chaîne dans laquelle chercher, c'est sûr la chaîne ne peut pas finir avec.
		if(len2 > len1)
			return false;

		size_t diff = len1 - len2;

		return memcmp(toSearch + diff, end, (len1 - diff) * sizeof(*end)) == 0;
	}

	/*
	======================
	StringUtils::lastIndex
	======================
	*/
	size_t StringUtils::lastIndex(const wchar_t *str, wchar_t charactere)
	{
		size_t num = length(str);
		if(num <= 1)
			return 0;

		num--;

		while(1) {
			if(str[num] == charactere)
				break;

			if(num == 0)
				break;

			num--;
		}

		return num;
	}

	/*
	===================
	StringUtils::equals
	===================
	*/
	bool StringUtils::equals(const wchar_t *first, const wchar_t *second)
	{
		size_t len1 = length(first);
		size_t len2 = length(second);

		// Si la longueur des 2 chaînes n'est pas la même alors il est clair que les 2 chaînes ne sont pas identiques.
		if(len1 != len2)
			return false;

		return memcmp(first, second, len1 * sizeof(wchar_t)) == 0;
	}

	/*
	========================
	StringUtils::wcharToChar
	========================
	*/
	void StringUtils::wcharToChar(char *dest, const wchar_t *source)
	{
		size_t len = length(source);
		size_t i;

		for(i = 0; i < len; ++i)
			dest[i] = (source[i] & 0x00FF);
	}

	/*
	=================
	StringUtils::hash
	=================
	*/
	uint64_t StringUtils::hash(const char *str)
	{
		uint64_t val = 5381;
		char c;

		while((c = *str++) != '\0')
			val = ((val << 5) + val) + c; /* val * 33 + c */

		return val;
	}

}

size_t de_string_utils_copy(char *dest, size_t destSize, const char *source, size_t sourceLength) {
	return de::StringUtils::copy(dest, destSize, source, sourceLength);
}

int de_string_utils_to_upper(int value) {
	return de::StringUtils::toUpper(value);
}
