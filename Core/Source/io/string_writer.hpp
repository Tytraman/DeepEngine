#ifndef __DEEP_ENGINE_STRING_WRITER_HPP__
#define __DEEP_ENGINE_STRING_WRITER_HPP__

#include "io/text_writer.hpp"
#include "core/string/string_utils.hpp"
#include "core/string/string.hpp"

namespace deep
{

    class string_writer : public text_writer
    {

        public:
            DE_API string_writer();

            DE_API bool open() override;
            DE_API bool close() override;

            DE_API virtual bool write(bool value) override;
            DE_API virtual bool write(char value) override;
            DE_API virtual bool write(const char *str) override;
            DE_API virtual bool write(const char *str, size_t offset, size_t count) override;
            DE_API virtual bool write(int32_t value) override;
            DE_API virtual bool write(uint32_t value) override;
            DE_API virtual bool write(int64_t value) override;
            DE_API virtual bool write(uint64_t value) override;
            DE_API virtual bool write(unsigned long value) override;
            DE_API virtual bool write(double value) override;
            DE_API virtual bool write(mem_ptr ptr) override;

            DE_API virtual bool is_opened() const override;

            DE_API string &get_content();

        protected:
            string m_Content;

    };

    /*
    ===================
    string_writer::open
    ===================
    */
    inline bool string_writer::open()
    {
        return true;
    }

    /*
    ====================
    string_writer::close
    ====================
    */
    inline bool string_writer::close()
    {
        return true;
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(bool value)
    {
        string str = string_utils::bool_to_str(value);

        return m_Content.append(str);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(char value)
    {
        return m_Content.append(value);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(const char *str)
    {
        return m_Content.append(str);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(const char *str, size_t offset, size_t count)
    {
        return m_Content.append(add_const<uint8_t *>(str) + offset, count);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(int32_t value)
    {
        return write(static_cast<int64_t>(value));
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(uint32_t value)
    {
        return write(static_cast<uint64_t>(value));
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(int64_t value)
    {
        string str;

        switch(m_NumberFormat)
        {
            default: return false;
            case number_format::Decimal:
            {
                str = string_utils::int_to_str(value);
            } break;
            case number_format::Hexadecimal:
            {
                str = string_utils::uint_to_hex_str(static_cast<uint64_t>(value));
            } break;
        }
        
        return m_Content.append(str);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(uint64_t value)
    {
        string str;
        
        switch(m_NumberFormat)
        {
            default: return false;
            case number_format::Decimal:
            {
                str = string_utils::uint_to_str(value);
            } break;
            case number_format::Hexadecimal:
            {
                str = string_utils::uint_to_hex_str(value);
            } break;
        }

        return m_Content.append(str);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(unsigned long value)
    {
        return write(static_cast<uint64_t>(value));
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(double value)
    {
        string str = string_utils::double_to_str(value);

        return m_Content.append(str);
    }

    /*
    ====================
    string_writer::write
    ====================
    */
    inline bool string_writer::write(mem_ptr ptr)
    {
        return write(reinterpret_cast<uint64_t>(ptr));
    }

    /*
    ========================
    string_writer::is_opened
    ========================
    */
    inline bool string_writer::is_opened() const
    {
        return true;
    }

    /*
    ==========================
    string_writer::get_content
    ==========================
    */
    inline string &string_writer::get_content()
    {
        return m_Content;
    }

}

#endif