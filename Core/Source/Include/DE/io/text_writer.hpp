#ifndef __DEEP_ENGINE_TEXT_WRITER_HPP__
#define __DEEP_ENGINE_TEXT_WRITER_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"

namespace deep
{

    class text_writer
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
            virtual bool write(double value) = 0;

            text_writer &operator<<(bool value);
            text_writer &operator<<(char value);
            text_writer &operator<<(const char *str);
            text_writer &operator<<(int32_t value);
            text_writer &operator<<(uint32_t value);
            text_writer &operator<<(int64_t value);
            text_writer &operator<<(uint64_t value);
            text_writer &operator<<(double value);

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
    inline text_writer &text_writer::operator<<(double value)
    {
        write(value);

        return *this;
    }

}

#endif