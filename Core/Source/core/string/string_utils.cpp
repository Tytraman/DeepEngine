#include "core/string/string_utils.hpp"
#include "core/string/string.hpp"
#include "core/memory.hpp"
#include "core/templates/list.hpp"

#include <stdio.h>
#include <cmath>

namespace deep
{

    /*
    ==================
    string_utils::copy
    ==================
    */
    size_t string_utils::copy(char *dest, size_t destSize, const char *source, size_t sourceLength)
    {
        size_t len = 0;
    
        if(destSize == 0)
        {
            return 0;
        }

        while(len < sourceLength)
        {
            if(len == destSize - 1)
            {
                break;
            }

            dest[len] = source[len];
            len++;
        }

        dest[len] = '\0';

        return len;
    }

    /*
    ==================
    string_utils::copy
    ==================
    */
    char *string_utils::copy(const char *source)
    {
        if(source == nullptr)
        {
            return nullptr;
        }

        size_t len = length(source);
        char *dest = (char *) mem::alloc(len + 1);

        if(dest == nullptr)
        {
            return nullptr;
        }

        memcpy(dest, source, len);
        dest[len] = '\0';

        return dest;
    }

    /*
    ==================
    string_utils::copy
    ==================
    */
    wchar_t *string_utils::copy(const wchar_t *source)
    {
        size_t len = length(source);
        wchar_t *dest = (wchar_t *) mem::alloc((len * sizeof(wchar_t)) + sizeof(wchar_t));

        if(dest == nullptr)
        {
            return nullptr;
        }

        memcpy(dest, source, len * sizeof(wchar_t));
        dest[len] = L'\0';

        return dest;
    }

    /*
    ===========================
    string_utils::new_substring
    ===========================
    */
    char *string_utils::new_substring(const char *source, size_t start, size_t end)
    {
        if(start >= end)
        {
            return nullptr;
        }

        size_t len = length(source);

        if(
            start >= len ||
            end > len
        )
        {
            return nullptr;
        }
            

        size_t size = end - start;

        char *str = static_cast<char *>(mem::alloc(size * sizeof(char) + sizeof(char)));
        if(str == nullptr)
        {
            return nullptr;
        }

        memcpy(str, source + start, size * sizeof(char));
        str[size] = '\0';

        return str;
    }

    /*
    =======================
    string_utils::substring
    =======================
    */
    bool string_utils::substring(char **str, size_t start, size_t end)
    {
        if(start >= end)
        {
            return false;
        }

        size_t len = length(*str);
        if(
            start >= len ||
            end > len
        )
        {
            return false;
        }

        size_t size = end - start;
        mem_ptr ptr = mem::realloc(*str, size * sizeof(char) + sizeof(char));
        if(ptr == nullptr)
        {
            return false;
        }

        *str = static_cast<char *>(ptr);
        str[size] = '\0';

        return true;
    }

    /*
    ===================
    string_utils::rtrim
    ===================
    */
    bool string_utils::rtrim(char **str, char charactere)
    {
        size_t len = length(*str);

        if(len == 0)
        {
            return false;
        }

        len--;

        while(true)
        {
            if((*str)[len] != charactere)
            {
                break;
            }

            if(len == 0)
            {
                break;
            }

            len--;
        }

        return substring(str, 0, len);
    }

    /*
    ======================
    string_utils::to_upper
    ======================
    */
    int string_utils::to_upper(int value)
    {
        if(value >= 'a' && value <= 'z')
        {
            return value - 32;
        }
            
        return value;
    }

    uint64_t string_utils::hash(const char *str)
    {
        uint64_t val = 5381;
        size_t len = length(str);
        size_t i;

        for(i = 0; i < len; ++i)
        {
            val = ((val << 5) + val) + str[i]; /* val * 33 + chars[i] */
        }

        return val;
    }

    /*
    ====================
    string_utils::length
    ====================
    */
    size_t string_utils::length(const char *str)
    {
        if(str == nullptr)
        {
            return 0;
        }

        size_t len = 0;

        while(str[len] != '\0')
        {
            len++;
        }

        return len;
    }

    /*
    ====================
    string_utils::length
    ====================
    */
    size_t string_utils::length(const wchar_t *str)
    {
        size_t len = 0;

        while(str[len] != L'\0')
        {
            len++;
        }

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
        {
            return false;
        }

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
        {
            return false;
        }

        *dest = static_cast<wchar_t *>(ptr);

        memcpy(*dest + destLen, source, sourceLen * sizeof(wchar_t));

        *(*dest + len) = L'\0';

        return true;
    }

    /*
    ====================
    string_utils::append
    ====================
    */
    bool string_utils::append(wchar_t **dest, const char *source)
    {
        size_t destLen = length(*dest);
        size_t sourceLen = length(source);
        size_t len = destLen + sourceLen;

        mem_ptr ptr = mem::realloc(*dest, len * sizeof(wchar_t) + sizeof(wchar_t));

        if(ptr == nullptr)
        {
            return false;
        }

        *dest = (wchar_t *) ptr;

        size_t i;
        for(i = 0; i < sourceLen; ++i)
        {
            (*dest)[destLen] = (wchar_t) source[i];
            ++destLen;
        }

        *(*dest + len) = L'\0';

        return true;
    }

    /*
    ====================
    string_utils::append
    ====================
    */
    bool string_utils::append(char **dest, uint8_t *buffer, size_t size)
    {
        size_t destLen;

        if(*dest != nullptr)
        {
            destLen = length(*dest);
        }
        else
        {
            destLen = 0;
        }

        size_t len = destLen + size;

        mem_ptr ptr = mem::realloc(*dest, len * sizeof(char) + sizeof(char));

        if(ptr == nullptr)
        {
            return false;
        }

        *dest = static_cast<char *>(ptr);

        memcpy(*dest + destLen, buffer, size);

        *(*dest + len) = '\0';

        return true;
    }

    /*
    ========================
    string_utils::remove_all
    ========================
    */
    size_t string_utils::remove_all(char **str, char caractere)
    {
        if(*str == nullptr)
        {
            return 0;
        }

        size_t len = length(*str);
        size_t backupLen = len;
        size_t index = 0;

        while(index < len)
        {
            if((*str)[index] == caractere)
            {
                size_t diff = len - index;

                memmove(*str + index * sizeof(char), *str + index * sizeof(char) + sizeof(char), diff * sizeof(char));

                len--;
            }

            index++;
        }

        if(len < backupLen)
        {
            mem_ptr ptr = mem::realloc(*str, len * sizeof(char) + sizeof(char));

            if(ptr != nullptr)
            {
                *str = static_cast<char *>(ptr);
            }
                
            (*str)[len] = '\0';
        }

        return backupLen - len;
    }

    /*
    =========================
    string_utils::replace_all
    =========================
    */
    size_t string_utils::replace_all(char *str, char toReplace, char replacement)
    {
        if(str == nullptr)
        {
            return 0;
        }

        size_t number = 0;
        size_t len = length(str);
        size_t index;

        for(index = 0; index < len; ++index)
        {
            if(str[index] == toReplace)
            {
                str[index] = replacement;

                number++;
            }
        }

        return number;
    }

    /*
    =======================
    string_utils::ends_with
    =======================
    */
    bool string_utils::ends_with(const char *toSearch, const char *end)
    {
        size_t len1 = length(toSearch);
        size_t len2 = length(end);

        // Si la chaîne de fin est plus longue que la chaîne dans laquelle chercher, c'est sûr la chaîne ne peut pas finir avec.
        if(len2 > len1)
        {
            return false;
        }

        size_t diff = len1 - len2;

        return memcmp(toSearch + diff, end, (len1 - diff) * sizeof(*end)) == 0;
    }

    /*
    =======================
    string_utils::ends_with
    =======================
    */
    bool string_utils::ends_with(const wchar_t *toSearch, const wchar_t *end)
    {
        size_t len1 = length(toSearch);
        size_t len2 = length(end);

        // Si la chaîne de fin est plus longue que la chaîne dans laquelle chercher, c'est sûr la chaîne ne peut pas finir avec.
        if(len2 > len1)
        {
            return false;
        }

        size_t diff = len1 - len2;

        return memcmp(toSearch + diff, end, (len1 - diff) * sizeof(*end)) == 0;
    }

    /*
    ========================
    string_utils::last_index
    ========================
    */
    size_t string_utils::last_index(const char *str, char charactere)
    {
        size_t num = length(str);

        if(num <= 1)
        {
            return 0;
        }

        num--;

        while(1)
        {
            if(str[num] == charactere)
            {
                break;
            }

            if(num == 0)
            {
                break;
            }

            num--;
        }

        return num;
    }

    /*
    ========================
    string_utils::last_index
    ========================
    */
    size_t string_utils::last_index(const wchar_t *str, wchar_t charactere)
    {
        size_t num = length(str);

        if(num <= 1)
        {
            return 0;
        }

        num--;

        while(1)
        {
            if(str[num] == charactere)
            {
                break;
            }

            if(num == 0)
            {
                break;
            }

            num--;
        }

        return num;
    }

    /*
    ====================
    string_utils::equals
    ====================
    */
    bool string_utils::equals(const wchar_t *first, const wchar_t *second)
    {
        size_t len1 = length(first);
        size_t len2 = length(second);

        // Si la longueur des 2 chaînes n'est pas la même alors il est clair que les 2 chaînes ne sont pas identiques.
        if(len1 != len2)
        {
            return false;
        }

        return memcmp(first, second, len1 * sizeof(wchar_t)) == 0;
    }

    /*
    ====================
    string_utils::equals
    ====================
    */
    bool string_utils::equals(const char *first, const char *second)
    {
        if(first == nullptr && second == nullptr)
        {
            return true;
        }

        if(first == nullptr)
        {
            return false;
        }

        if(second == nullptr)
        {
            return false;
        }

        size_t len1 = length(first);
        size_t len2 = length(second);

        // Si la longueur des 2 chaînes n'est pas la même alors il est clair que les 2 chaînes ne sont pas identiques.
        if(len1 != len2)
        {
            return false;
        }

        return memcmp(first, second, len1 * sizeof(char)) == 0;
    }

    /*
    ===========================
    string_utils::wchar_to_char
    ===========================
    */
    void string_utils::wchar_to_char(char *dest, const wchar_t *source)
    {
        size_t len = length(source);
        size_t i;

        for(i = 0; i < len; ++i)
            dest[i] = (source[i] & 0x00FF);

        dest[i] = '\0';
    }

    /*
    ==============================
    string_utils::uint_to_hex_char
    ==============================
    */
    char string_utils::uint_to_hex_char(uint8_t value)
    {
        switch(value)
        {
            default: return -1;
            case 0:  return '0';
            case 1:  return '1';
            case 2:  return '2';
            case 3:  return '3';
            case 4:  return '4';
            case 5:  return '5';
            case 6:  return '6';
            case 7:  return '7';
            case 8:  return '8';
            case 9:  return '9';
            case 10: return 'A';
            case 11: return 'B';
            case 12: return 'C';
            case 13: return 'D';
            case 14: return 'E';
            case 15: return 'F';
        }
    }

    /*
    =========================
    string_utils::bool_to_str
    =========================
    */
    string string_utils::bool_to_str(bool value)
    {
        string str;

        switch(value)
        {
            case false:
            {
                str = "false";
            } break;
            case true:
            {
                str = "true";
            } break;
        }

        return str;
    }

    /*
    =========================
    string_utils::uint_to_str
    =========================
    */
    string string_utils::uint_to_str(uint64_t value)
    {
        string str;

        if(value < 10)
        {
            str.append(static_cast<char>(value + '0'));

            return str;
        }

        uint8_t len = 0;
        uint64_t copy = value;

        // Récupère le nombre de chiffres du numéro.
        while(copy != 0)
        {
            copy /= 10;
            len++;
        }

        if(!str.reserve(len * sizeof(char) + sizeof(char)))
        {
            return str;
        }

        str.set_length(len);
        str[len] = '\0';

        len--;

        while(value != 0)
        {
            str[len] = value % 10 + '0';
            value /= 10;
            len--;
        }

        return str;
    }

    /*
    =============================
    string_utils::uint_to_hex_str
    =============================
    */
    string string_utils::uint_to_hex_str(uint64_t value)
    {
        string str;

        if(value < 16)
        {
            str.append(uint_to_hex_char(static_cast<uint8_t>(value)));

            return str;
        }

        uint8_t len = 0;
        uint64_t copy = value;

        // Récupère le nombre de chiffres du numéro.
        while(copy != 0)
        {
            copy /= 10;
            len += 2;
        }

        if(!str.reserve(len * sizeof(char) + sizeof(char)))
        {
            return str;
        }

        str.set_length(len);
        str[len] = '\0';

        len--;

        uint8_t digit;

        while(value != 0)
        {
            digit = value % 10;

            str[len]     = uint_to_hex_char(digit & 0x0F);
            str[len - 1] = uint_to_hex_char((digit >> 4) & 0x0F);

            value /= 10;
            len -= 2;
        }

        return str;
    }

    /*
    ========================
    string_utils::int_to_str
    ========================
    */
    string string_utils::int_to_str(int64_t value)
    {
        string str;

        auto ab = std::abs(value);

        if(value > -10 && value < 10)
        {
            if(value < 0)
            {
                str.append('-');
            }

            str.append(static_cast<char>(ab + '0'));

            return str;
        }

        uint8_t len = 0;
        auto copy = ab;

        while(copy != 0)
        {
            copy /= 10;
            len++;
        }

        if(value < 0)
        {
            len++;
        }

        if(!str.reserve(len * sizeof(char) + sizeof(char)))
        {
            return str;
        }

        str.set_length(len);
        str[len] = '\0';

        len--;

        while(ab != 0)
        {
            str[len] = ab % 10 + '0';
            ab /= 10;
            len--;
        }

        if(value < 0)
        {
            str[0] = '-';
        }

        return str;
    }

    /*
    ===========================
    string_utils::double_to_str
    ===========================
    */
    string string_utils::double_to_str(double value)
    {
        string str;
        uint16_t len = 0;

        // Récupère la partie entière.
        double integer;
        double decimal = std::modf(value, &integer);
        integer = std::abs(integer);

        if(value > -10.0 && value < 10.0)
        {
            if(value < 0.0)
            {
                str.append('-');

                len++;
            }

            str.append(static_cast<char>(static_cast<uint8_t>(integer) + '0'));

            len++;
        }
        else
        {
            uint64_t copy = static_cast<uint64_t>(integer);

            while(copy != 0)
            {
                copy /= 10;
                len++;
            }

            if(value < 0.0)
            {
                len++;
            }

            if(!str.reserve(len * sizeof(char) + sizeof(char)))
            {
                return str;
            }
            
            str.set_length(len);

            len--;

            copy = static_cast<uint64_t>(integer);

            while(copy != 0)
            {
                str[len] = copy % 10 + '0';
                copy /= 10;
                len--;
            }

            if(value < 0)
            {
                str[0] = '-';
            }
        }

        // S'occupe ensuite de la partie décimale.
        str.append('.');

        if(decimal == 0.0)
        {
            str.append('0');

            return str;
        }

        while(decimal != 0.0)
        {
            decimal *= 10;
            decimal = std::modf(decimal, &integer);
            integer = std::abs(integer);

            str.append(static_cast<char>(static_cast<uint8_t>(integer) + '0'));
        }

        return str;
    }

    /*
    ===================
    string_utils::split
    ===================
    */
    size_t string_utils::split(const char *str, char separator, list<string> &dest)
    {
        size_t i, number = 1;
        size_t lastFound = 0;
        size_t len = length(str);
        const char *current = str;

        for(i = 0; i < len; ++i)
        {
            if(str[i] == separator)
            {
                dest.add(string(current, i - lastFound));

                current = str + i + 1;
                lastFound = i + 1;

                number++;
            }
        }

        dest.add(string(current, i - lastFound));

        return number;
    }

    /*
    ===================================
    string_utils::is_null_or_whitespace
    ===================================
    */
    bool string_utils::is_null_or_whitespace(const char *str)
    {
        if(str == nullptr)
        {
            return true;
        }

        while(*str != '\0')
        {
            if(
                *str != ' '  &&
                *str != '\t' &&
                *str != '\n' &&
                *str != '\v' &&
                *str != '\f' &&
                *str != '\r'
                )
            {
                return false;
            }

            str++;
        }

        return true;
    }

}
