#ifndef __DEEP_ENGINE_STRING_UTILS_HPP__
#define __DEEP_ENGINE_STRING_UTILS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>

namespace deep
{

	class DE_API string_utils
    {

		public:
			static size_t copy(char *dest, size_t destSize, const char *source, size_t sourceLength);

			/// @brief        Copie une chaîne de caractères dans un espace mémoire alloué dynamiquement, doit être \ref mem:free.
			/// @param source Chaîne de caractères à copier.
			/// @return       La chaîne dynamiquement créée ou \c nullptr si une erreur est survenue.
			static char *copy(const char *source);
			static wchar_t *copy(const wchar_t *source);

            static char *substring(const char *source, size_t start, size_t end);

            static bool append(char **dest, const char *source);
			static bool append(wchar_t **dest, const wchar_t *source);
			static bool append(wchar_t **dest, const char *source);
            static bool append(char **dest, uint8_t *buffer, size_t size);

            static size_t removeAll(char **str, char caractere);

            static bool endsWith(const char *toSearch, const char *end);
			static bool endsWith(const wchar_t *toSearch, const wchar_t *end);

			static size_t lastIndex(const char *str, char charactere);
            static size_t lastIndex(const wchar_t *str, wchar_t charactere);

			static bool equals(const wchar_t *first, const wchar_t *second);
            static bool equals(const char *first, const char *second);

			static void wcharToChar(char *dest, const wchar_t *source);

			static int toUpper(int value);

            static uint64_t hash(const char *str);

			static size_t length(const char *str);
			static size_t length(const wchar_t *str);

        public:
            string_utils() = delete;

	};

}

#endif