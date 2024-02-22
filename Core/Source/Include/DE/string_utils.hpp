#ifndef __DEEP_ENGINE_STRING_UTILS_HPP__
#define __DEEP_ENGINE_STRING_UTILS_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"

namespace deep
{

    class string;

    class string_utils
    {

        public:
            DE_API static size_t copy(char *dest, size_t destSize, const char *source, size_t sourceLength);

            /// @brief        Copie une chaîne de caractères dans un espace mémoire alloué dynamiquement, doit être \ref mem:free.
            /// @param source Chaîne de caractères à copier.
            /// @return       La chaîne dynamiquement créée ou \c nullptr si une erreur est survenue.
            DE_API static char *copy(const char *source);
            DE_API static wchar_t *copy(const wchar_t *source);

            DE_API static char *new_substring(const char *source, size_t start, size_t end);
            DE_API static bool substring(char **str, size_t start, size_t end);

            DE_API static bool rtrim(char **str, char charactere);

            DE_API static bool append(char **dest, const char *source);
            DE_API static bool append(wchar_t **dest, const wchar_t *source);
            DE_API static bool append(wchar_t **dest, const char *source);
            DE_API static bool append(char **dest, uint8_t *buffer, size_t size);

            DE_API static size_t remove_all(char **str, char charactere);

            DE_API static size_t replace_all(char *str, char toReplace, char replacement);

            DE_API static bool ends_with(const char *toSearch, const char *end);
            DE_API static bool ends_with(const wchar_t *toSearch, const wchar_t *end);

            DE_API static size_t last_index(const char *str, char charactere);
            DE_API static size_t last_index(const wchar_t *str, wchar_t charactere);

            DE_API static bool equals(const wchar_t *first, const wchar_t *second);
            DE_API static bool equals(const char *first, const char *second);

            DE_API static void wchar_to_char(char *dest, const wchar_t *source);

            DE_API static int to_upper(int value);

            DE_API static uint64_t hash(const char *str);

            DE_API static size_t length(const char *str);
            DE_API static size_t length(const wchar_t *str);

            DE_API static string bool_to_str(bool value);
            DE_API static string uint_to_str(uint64_t value);

        public:
            string_utils() = delete;

    };

}

#endif