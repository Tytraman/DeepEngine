#ifndef __DEEP_ENGINE_STRING_HPP__
#define __DEEP_ENGINE_STRING_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/string_utils.hpp>

namespace deep
{

    class string;
    class string_finder;

    class DE_API string
    {

        public:
            string();
            string(const char *str);
            string(mem_ptr rawStr);
            string(const string &other);
            string(string &&other) noexcept;

            string &operator=(const string &other);
            string &operator=(string &&other) noexcept;
            string &operator=(const char *str);

            char operator[](size_t index) const;

            void clear();

            bool append(const char *str);
            bool append(uint8_t *buffer, size_t size);

            size_t removeAll(char caractere);

            string newSubstring(size_t start, size_t end) const;
            bool substring(size_t start, size_t end);

            size_t find(char charactere) const;
            size_t findFromEnd(char charactere) const;

            static uint64_t hash(const string &str);
            uint64_t hash() const;

            bool equals(const char *str) const;

            // ===== Converter ===== //

            bool toBool() const;
            uint8_t toUint8() const;
            uint16_t toUint16() const;
            uint32_t toUint32() const;
            uint64_t toUint64() const;

            size_t length() const;
            const char *str() const;

        private:
            unique_ptr<char> m_Chars;
            size_t m_Length;

    };

    /*
    ==================
    string::operator[]
    ==================
    */
    inline char string::operator[](size_t index) const
    {
        return m_Chars.get()[index];
    }

    /*
    ============
    string::hash
    ============
    */
    inline uint64_t string::hash(const string &str)
    {
        return str.hash();
    }

    /*
    ==============
    string::length
    ==============
    */
    inline size_t string::length() const
    {
        return m_Length;
    }

    /*
    ===========
    string::str
    ===========
    */
    inline const char *string::str() const
    {
        return m_Chars.get();
    }

    /*
    =============
    string::clear
    =============
    */
    inline void string::clear()
    {
        m_Chars.reset(nullptr);
        m_Length = 0;
    }

    /*
    ==============
    string::append
    ==============
    */
    inline bool string::append(const char *str)
    {
        return append((uint8_t *) str, string_utils::length(str));
    }

    /*
    ==============
    string::equals
    ==============
    */
    inline bool string::equals(const char *str) const
    {
        return string_utils::equals(m_Chars.get(), str);
    }

    /*
    ===============
    string::toUint8
    ===============
    */
    inline uint8_t string::toUint8() const
    {
        return static_cast<uint8_t>(toUint64());
    }

    /*
    ================
    string::toUint16
    ================
    */
    inline uint16_t string::toUint16() const
    {
        return static_cast<uint16_t>(toUint64());
    }

    /*
    ================
    string::toUint32
    ================
    */
    inline uint32_t string::toUint32() const
    {
        return static_cast<uint32_t>(toUint64());
    }

    class DE_API string_finder
    {

        public:
            string_finder(const string &str, size_t index = 0);

            char operator[](size_t index) const;

            bool skipWhiteChars();
            bool skipAlpha();
            bool skipNum();
            bool skipAlphaNum();
            bool skipTo(char charactere);
            bool skip();
            bool skip(char charactere);
            char skipUntil(char *characteres, uint8_t numberOfCharacteres);

            bool isAlphaNum() const;


            size_t position() const;
            char current() const;

            void setPosition(size_t index);

        private:
            const string *m_String;
            size_t m_Position;

        public:
            string_finder() = delete;

    };

    /*
    =========================
    string_finder::operator[]
    =========================
    */
    inline char string_finder::operator[](size_t index) const
    {
        return (*m_String)[index];
    }

    /*
    ===================
    string_finder::skip
    ===================
    */
    inline bool string_finder::skip()
    {
        if(m_Position < m_String->length())
        {
            m_Position++;
            return true;
        }

        return false;
    }

    /*
    =======================
    string_finder::position
    =======================
    */
    inline size_t string_finder::position() const
    {
        return m_Position;
    }

    /*
    ======================
    string_finder::current
    ======================
    */
    inline char string_finder::current() const
    {
        return (*m_String)[m_Position];
    }

    /*
    ==========================
    string_finder::setPosition
    ==========================
    */
    inline void string_finder::setPosition(size_t index)
    {
        m_Position = index;
    }

}

#endif