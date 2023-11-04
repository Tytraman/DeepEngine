#ifndef __DEEP_ENGINE_STRING_HPP__
#define __DEEP_ENGINE_STRING_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/string_utils.hpp>

namespace de
{

    class DE_API string
    {

        public:
            string();
            string(const char *str);
            string(const string &other);
            string(string &&other) noexcept;

            string &operator=(const string &other);
            string &operator=(string &&other) noexcept;

            void clear();

            bool append(const char *str);

            static uint64_t hash(const string &str);
            uint64_t hash() const;

            size_t length() const;
            const char *str() const;

        private:
            unique_ptr<char> m_Chars;
            size_t m_Length;

    };

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

    inline void string::clear()
    {
        m_Chars.reset(nullptr);
        m_Length = 0;
    }

    inline bool string::append(const char *str)
    {
        char *ptr = m_Chars.get();

        return string_utils::append(&ptr, str);
    }

}

#endif