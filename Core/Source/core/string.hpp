#ifndef __DEEP_ENGINE_STRING_HPP__
#define __DEEP_ENGINE_STRING_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/memory.hpp"
#include "core/string_utils.hpp"

namespace deep
{

    class string;
    class string_finder;

    class string
    {

        public:
            DE_API string();
            DE_API string(const char *str);
            DE_API string(const char *str, size_t count);
            DE_API string(mem_ptr rawStr);
            DE_API string(const string &other);
            DE_API string(string &&other) noexcept;

            DE_API string &operator=(const string &other);
            DE_API string &operator=(string &&other) noexcept;
            DE_API string &operator=(const char *str);

            DE_API char &operator[](size_t index) const;

            DE_API void clear();

            DE_API bool append(const char *str);
            DE_API bool append(uint8_t *buffer, size_t size);
            DE_API bool append(char charactere);

            DE_API size_t remove_all(char charactere);

            DE_API string new_substring(size_t start, size_t end) const;
            DE_API bool substring(size_t start, size_t end);

            DE_API bool rtrim(char toRemove);

            DE_API size_t find(char charactere) const;
            DE_API size_t find_from_end(char charactere) const;

            DE_API static uint64_t hash(const string &str);
            DE_API uint64_t hash() const;

            DE_API bool equals(const char *str) const;
            DE_API bool ends_with(const char *str) const;

            DE_API void set_ptr(char *ptr);
            DE_API void set_length(size_t len);

            DE_API bool reserve(size_t len);

            // ===== Converter ===== //

            DE_API bool to_bool() const;
            DE_API uint8_t to_uint8() const;
            DE_API uint16_t to_uint16() const;
            DE_API uint32_t to_uint32() const;
            DE_API uint64_t to_uint64() const;

            DE_API size_t length() const;
            DE_API const char *str() const;

        private:
            unique_ptr<char> m_Chars;
            size_t m_Length;

    };

    /*
    ==============
    string::append
    ==============
    */
    inline bool string::append(char charactere)
    {
        return append(add_const<uint8_t *>(&charactere), static_cast<size_t>(1));
    }

    /*
    ==================
    string::operator[]
    ==================
    */
    inline char &string::operator[](size_t index) const
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
    =================
    string::ends_with
    =================
    */
    inline bool string::ends_with(const char *str) const
    {
        return string_utils::ends_with(m_Chars.get(), str);
    }

    /*
    ===============
    string::to_uint8
    ===============
    */
    inline uint8_t string::to_uint8() const
    {
        return static_cast<uint8_t>(to_uint64());
    }

    /*
    ================
    string::to_uint16
    ================
    */
    inline uint16_t string::to_uint16() const
    {
        return static_cast<uint16_t>(to_uint64());
    }

    /*
    ================
    string::to_uint32
    ================
    */
    inline uint32_t string::to_uint32() const
    {
        return static_cast<uint32_t>(to_uint64());
    }

    /*
    ===============
    string::set_ptr
    ===============
    */
    inline void string::set_ptr(char *ptr)
    {
        m_Chars.reset(ptr);
    }

    /*
    ==================
    string::set_length
    ==================
    */
    inline void string::set_length(size_t len)
    {
        m_Length = len;
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