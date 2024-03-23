#ifndef __DEEP_ENGINE_TEXT_WRITER_HPP__
#define __DEEP_ENGINE_TEXT_WRITER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/ref_counted.hpp"

namespace deep
{

    class text_writer : public ref_counted
    {

        public:
            virtual bool open() = 0;
            virtual bool close() = 0;

            virtual bool write(bool value) = 0;
            virtual bool write(char value) = 0;
            virtual bool write(const char *str) = 0;
            virtual bool write(const char *str, size_t offset, size_t count) = 0;
            virtual bool write(int32_t value) = 0;
            virtual bool write(uint32_t value) = 0;
            virtual bool write(int64_t value) = 0;
            virtual bool write(uint64_t value) = 0;
            virtual bool write(unsigned long value) = 0;
            virtual bool write(double value) = 0;
            virtual bool write(mem_ptr ptr) = 0;

            DE_API text_writer &operator<<(bool value);
            DE_API text_writer &operator<<(char value);
            DE_API text_writer &operator<<(const char *str);
            DE_API text_writer &operator<<(int32_t value);
            DE_API text_writer &operator<<(uint32_t value);
            DE_API text_writer &operator<<(int64_t value);
            DE_API text_writer &operator<<(uint64_t value);
            DE_API text_writer &operator<<(unsigned long value);
            DE_API text_writer &operator<<(double value);
            DE_API text_writer &operator<<(mem_ptr ptr);
            
            DE_API text_writer &operator<<(number_format format);

        protected:
            text_writer();

        protected:
            number_format m_NumberFormat;

    };

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(bool value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(char value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(const char *str)
    {
        write(str);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(int32_t value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(uint32_t value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(int64_t value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(uint64_t value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(unsigned long value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(double value)
    {
        write(value);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(mem_ptr ptr)
    {
        write(ptr);

        return *this;
    }

    /*
    =======================
    text_writer::operator<<
    =======================
    */
    inline text_writer &text_writer::operator<<(number_format format)
    {
        m_NumberFormat = format;

        return *this;
    }

}

#endif