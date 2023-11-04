#include <DE/string_utils.hpp>
#include <DE/memory/memory.hpp>

#include <DE/c-wrapper/string_utils.h>

#include <string.h>
#include <stdio.h>

namespace de {

	/*
	=================
	string_utils::copy
	=================
	*/
	size_t string_utils::copy(char *dest, size_t destSize, const char *source, size_t sourceLength) {
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
	string_utils::copy
	=================
	*/
	char *string_utils::copy(const char *source)
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
	string_utils::copy
	=================
	*/
	wchar_t *string_utils::copy(const wchar_t *source)
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
	string_utils::toUpper
	====================
	*/
	int string_utils::toUpper(int value) {
		if(value >= 'a' && value <= 'z')
			return value - 32;
		return value;
	}

	/*
	===================
	string_utils::length
	===================
	*/
	size_t string_utils::length(const char *str)
	{
		size_t len = 0;

		while(str[len] != '\0')
			len++;

		return len;
	}

	/*
	===================
	string_utils::length
	===================
	*/
	size_t string_utils::length(const wchar_t *str)
	{
		size_t len = 0;

		while(str[len] != L'\0')
			len++;

		return len;
	}

    /*
	====================
	string_utils::append
	====================
	*/
    bool string_utils::append(char **dest, const char *source)
    {
        size_t destLen = length(*dest);
		size_t sourceLen = length(source);
		size_t len = destLen + sourceLen;

        mem_ptr ptr = mem::realloc(*dest, len * sizeof(char) + sizeof(char));
        if(ptr == nullptr)
			return false;

        *dest = static_cast<char *>(ptr);

        memcpy(*dest + destLen, source, sourceLen * sizeof(char));

        *(*dest + len) = '\0';

        return true;
    }

	/*
	====================
	string_utils::append
	====================
	*/
	bool string_utils::append(wchar_t **dest, const wchar_t *source)
	{
		size_t destLen = length(*dest);
		size_t sourceLen = length(source);
		size_t len = destLen + sourceLen;

		mem_ptr ptr = mem::realloc(*dest, len * sizeof(wchar_t) + sizeof(wchar_t));
		if(ptr == nullptr)
			return false;

		*dest = static_cast<wchar_t *>(ptr);

		memcpy(*dest + destLen, source, sourceLen * sizeof(wchar_t));

		*(*dest + len) = L'\0';

		return true;
	}

	/*
	===================
	string_utils::append
	===================
	*/
	bool string_utils::append(wchar_t **dest, const char *source)
	{
		size_t destLen = length(*dest);
		size_t sourceLen = length(source);
		size_t len = destLen + sourceLen;

		mem_ptr ptr = mem::realloc(*dest, len * sizeof(wchar_t) + sizeof(wchar_t));
		if(ptr == nullptr)
			return false;

		*dest = (wchar_t *) ptr;

		size_t i;
		for(i = 0; i < sourceLen; ++i) {
			(*dest)[destLen] = (wchar_t) source[i];
			++destLen;
		}

		*(*dest + len) = L'\0';

		return true;
	}

	/*
	=====================
	string_utils::endsWith
	=====================
	*/
	bool string_utils::endsWith(const wchar_t *toSearch, const wchar_t *end)
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
	string_utils::lastIndex
	======================
	*/
	size_t string_utils::lastIndex(const wchar_t *str, wchar_t charactere)
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
	string_utils::equals
	===================
	*/
	bool string_utils::equals(const wchar_t *first, const wchar_t *second)
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
	string_utils::wcharToChar
	========================
	*/
	void string_utils::wcharToChar(char *dest, const wchar_t *source)
	{
		size_t len = length(source);
		size_t i;

		for(i = 0; i < len; ++i)
			dest[i] = (source[i] & 0x00FF);

        dest[i] = '\0';
	}

}

size_t de_string_utils_copy(char *dest, size_t destSize, const char *source, size_t sourceLength) {
	return de::string_utils::copy(dest, destSize, source, sourceLength);
}

int de_string_utils_to_upper(int value) {
	return de::string_utils::toUpper(value);
}
