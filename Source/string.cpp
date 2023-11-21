#include <DE/string.hpp>
#include <DE/common.hpp>

namespace de
{

    /*
    ==============
    string::string
    ==============
    */
    string::string()
        : m_Chars(nullptr),
          m_Length(0)
    { }
    
    /*
    ==============
    string::string
    ==============
    */
    string::string(const char *str)
        : m_Chars(string_utils::copy(str)),
          m_Length(string_utils::length(str))
    { }

    /*
    ==============
    string::string
    ==============
    */
    string::string(mem_ptr rawStr)
        : m_Chars(static_cast<char *>(rawStr)),
          m_Length(string_utils::length(static_cast<char *>(rawStr)))
    { }

    /*
    ==============
    string::string
    ==============
    */
    string::string(const string &other)
        : m_Length(other.m_Length)
    {
        const char *chars = other.m_Chars.get();
        if(chars != nullptr)
            m_Chars = string_utils::copy(chars);
    }

    /*
    ==============
    string::string
    ==============
    */
    string::string(string &&other) noexcept
        : m_Length(exchange(other.m_Length, static_cast<size_t>(0))),
          m_Chars(other.m_Chars.release())
    { }

    /*
    =================
    string::operator=
    =================
    */
    string &string::operator=(const string &other)
    {
        if(this != &other)
        {
            const char *chars = other.m_Chars.get();

            if(chars != nullptr)
            {
                m_Length = other.m_Length;
                m_Chars = string_utils::copy(chars);
            }
        }

        return *this;
    }

    /*
    =================
    string::operator=
    =================
    */
    string &string::operator=(string &&other) noexcept
    {
        if(this != &other)
        {
            m_Length = exchange(other.m_Length, static_cast<size_t>(0));
            m_Chars = other.m_Chars.release();
        }

        return *this;
    }

    /*
    ==============
    string::append
    ==============
    */
    bool string::append(uint8_t *buffer, size_t size)
    {
        char *ptr = m_Chars.get();

        if(ptr != nullptr)
        {
            if(string_utils::append(&ptr, buffer, size))
            {
                m_Chars.assign(ptr);

                m_Length += size;

                return true;
            }
        }

        if(string_utils::append(&ptr, buffer, size))
        {
            m_Chars.assign(ptr);

            m_Length += size;

            return true;
        }

        return false;
    }

    /*
    ==============
    string::append
    ==============
    */
    size_t string::removeAll(char caractere)
    {
        char *ptr = m_Chars.get();

        size_t len = string_utils::removeAll(&ptr, caractere);
        m_Chars.assign(ptr);
        m_Length -= len;

        return len;
    }

    /*
    ====================
    string::newSubstring
    ====================
    */
    string string::newSubstring(size_t start, size_t end) const
    {
        char *str = string_utils::substring(m_Chars.get(), start, end);
        if(str == nullptr)
            return string("");

        return string(static_cast<mem_ptr>(str));
    }

    /*
    =================
    string::substring
    =================
    */
    bool string::substring(size_t start, size_t end)
    {
        char *str = string_utils::substring(m_Chars.get(), start, end);
        if(str == nullptr)
            return false;

        m_Length = end - start;
        m_Chars.reset(str);

        return true;
    }

    /*
    ============
    string::find
    ============
    */
    size_t string::find(char charactere) const
    {
        size_t len = m_Length;
        size_t index;
        char *text = m_Chars.get();

        for(index = 0; index < len; ++index)
        {
            if(text[index] == charactere)
                return index;
        }

        return -1;
    }

    /*
    ===================
    string::findFromEnd
    ===================
    */
    size_t string::findFromEnd(char charactere) const
    {
        if(m_Length == 0)
            return -1;

        size_t index = m_Length - 1;
        char *text = m_Chars.get();

        while(true)
        {
            if(text[index] == charactere)
                return index;

            if(index == 0)
                break;

            index--;
        }

        return -1;
    }

    /*
    ============
    string::hash
    ============
    */
    uint64_t string::hash() const
    {
        uint64_t val = 5381;
		size_t len = m_Length;
        size_t i;

        const char *chars = m_Chars.get();

        for(i = 0; i < len; ++i)
            val = ((val << 5) + val) + chars[i]; /* val * 33 + chars[i] */

		return val;
    }

    /*
    ============================
    string_finder::string_finder
    ============================
    */
    string_finder::string_finder(const string &str, size_t index)
        : m_String(&str),
          m_Position(index)
    { }

    /*
    =============================
    string_finder::skipWhiteChars
    =============================
    */
    bool string_finder::skipWhiteChars()
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            // Ignore tous les espaces, les tabulations, les retours chariots, les sauts de lignes et les caractères nuls.
            if(
                current != ' '  &&
                current != '\t' &&
                current != '\0')
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    ========================
    string_finder::skipAlpha
    ========================
    */
    bool string_finder::skipAlpha()
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            // Ignore toutes les lettres de l'alphabet.
            if(
                (current < 'A' || current > 'Z') &&
                (current < 'a' || current > 'z'))
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    ======================
    string_finder::skipNum
    ======================
    */
    bool string_finder::skipNum()
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            // Ignore tous les chiffres.
            if((current < '0' || current > '0'))
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    ======================
    string_finder::skipNum
    ======================
    */
    bool string_finder::skipAlphaNum()
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            // Ignore toutes les lettres de l'alphabet et les chiffres.
            if(
                (current < 'A' || current > 'Z') &&
                (current < 'a' || current > 'z') &&
                (current < '0' || current > '9'))
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    =====================
    string_finder::skipTo
    =====================
    */
    bool string_finder::skipTo(char charactere)
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            if((*m_String)[m_Position] == charactere)
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    ===================
    string_finder::skip
    ===================
    */
    bool string_finder::skip(char charactere)
    {
        size_t length = m_String->length();
        size_t pos = m_Position;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            // Ignore le caractère.
            if(current != charactere)
                break;

            m_Position++;
        }

        return m_Position > pos;
    }

    /*
    ========================
    string_finder::skipUntil
    ========================
    */
    char string_finder::skipUntil(char *characteres, uint8_t numberOfCharacteres)
    {
        size_t length = m_String->length();
        uint8_t index;

        while(m_Position < length)
        {
            char current = (*m_String)[m_Position];

            for(index = 0; index < numberOfCharacteres; ++index)
            {
                if(current == characteres[index])
                    return characteres[index];
            }

            m_Position++;
        }

        return -1;
    }

    /*
    =========================
    string_finder::isAlphaNum
    =========================
    */
    bool string_finder::isAlphaNum() const
    {
        char current = (*m_String)[m_Position];

        if(
            (current >= 'A' && current <= 'Z') ||
            (current >= 'a' && current <= 'z') ||
            (current >= '0' && current <= '9'))
            return true;

        return false;
    }

}
