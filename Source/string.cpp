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

}
