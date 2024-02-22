#ifndef __DEEP_ENGINE_STREAM_WRITER_HPP__
#define __DEEP_ENGINE_STREAM_WRITER_HPP__

#include "DE/io/text_writer.hpp"
#include "DE/io/stream.hpp"
#include "DE/string_utils.hpp"
#include "DE/string.hpp"

namespace deep
{

    class stream_writer : public text_writer
    {

        public:
            DE_API stream_writer(stream *stream);

            DE_API virtual bool write(bool value) override;
            DE_API virtual bool write(char value) override;
            DE_API virtual bool write(const char *str) override;
            DE_API virtual bool write(const char *str, size_t offset, size_t count) override;
            DE_API virtual bool write(int64_t value) override;
            DE_API virtual bool write(uint64_t value) override;
            DE_API virtual bool write(double value) override;

        private:
            stream *m_OutputStream;

        public:
            stream_writer() = delete;

    };

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(bool value)
    {
        string str = string_utils::bool_to_str(value);

        return m_OutputStream->write(rm_const<char *>(str.str()), 0, str.length(), nullptr);
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(char value)
    {
        return m_OutputStream->write(&value, 0, sizeof(value), nullptr);
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(const char */*str*/)
    {
        // TODO: à faire
        return true;
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(const char */*str*/, size_t /*offset*/, size_t /*count*/)
    {
        // TODO: à faire
        return true;
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(int64_t /*value*/)
    {
        // TODO: à faire
        return true;
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(uint64_t value)
    {
        string str = string_utils::uint_to_str(value);

        return m_OutputStream->write(rm_const<char *>(str.str()), 0, str.length(), nullptr);
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(double /*value*/)
    {
        // TODO: à faire
        return true;
    }

}

#endif