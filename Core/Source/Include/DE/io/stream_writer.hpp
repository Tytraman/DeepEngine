#ifndef __DEEP_ENGINE_STREAM_WRITER_HPP__
#define __DEEP_ENGINE_STREAM_WRITER_HPP__

#include "DE/io/text_writer.hpp"
#include "DE/io/stream.hpp"
#include "DE/string_utils.hpp"
#include "DE/string.hpp"

namespace deep
{

    /// @brief Classe héritant de \ref text_writer qui écrit du texte dans un stream.
    /// @remarks Le \ref stream_writer s'occupe de toute la gestion du stream, l'ouverture comme la fermeture.
    class stream_writer : public text_writer
    {

        public:
            DE_API stream_writer(stream *stream);

            DE_API bool open() override;
            DE_API bool close() override;

            DE_API stream_writer &operator=(const stream_writer &other);
            DE_API stream_writer &operator=(stream_writer &&other);

            DE_API virtual bool write(bool value) override;
            DE_API virtual bool write(char value) override;
            DE_API virtual bool write(const char *str) override;
            DE_API virtual bool write(const char *str, size_t offset, size_t count) override;
            DE_API virtual bool write(int32_t value) override;
            DE_API virtual bool write(uint32_t value) override;
            DE_API virtual bool write(int64_t value) override;
            DE_API virtual bool write(uint64_t value) override;
            DE_API virtual bool write(double value) override;

        private:
            stream *m_OutputStream;

        public:
            stream_writer() = delete;

    };

    /*
    ===================
    stream_writer::open
    ===================
    */
    inline bool stream_writer::open()
    {
        if(m_OutputStream == nullptr)
        {
            return false;
        }

        return m_OutputStream->open();
    }

    /*
    ====================
    stream_writer::close
    ====================
    */
    inline bool stream_writer::close()
    {
        if(m_OutputStream == nullptr)
        {
            return false;
        }

        if(m_OutputStream->close())
        {
            m_OutputStream = nullptr;

            return true;
        }
        else
        {
            return false;
        }
    }

    /*
    ========================
    stream_writer::operator=
    ========================
    */
    inline stream_writer &stream_writer::operator=(const stream_writer &other)
    {
        m_OutputStream->close();

        m_OutputStream = other.m_OutputStream;

        return *this;
    }

    /*
    ========================
    stream_writer::operator=
    ========================
    */
    inline stream_writer &stream_writer::operator=(stream_writer &&other)
    {
        m_OutputStream->close();

        m_OutputStream = other.m_OutputStream;
        other.m_OutputStream = nullptr;

        return *this;
    }

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
    inline bool stream_writer::write(const char *str)
    {
        size_t len = string_utils::length(str);

        return write(str, 0, len);
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(const char *str, size_t offset, size_t count)
    {
        return m_OutputStream->write(rm_const<char *>(str), offset, count, nullptr);
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(int32_t value)
    {
        return write(static_cast<int64_t>(value));
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(uint32_t value)
    {
        return write(static_cast<uint64_t>(value));
    }

    /*
    ====================
    stream_writer::write
    ====================
    */
    inline bool stream_writer::write(int64_t value)
    {
        string str = string_utils::int_to_str(value);
        
        return m_OutputStream->write(rm_const<char *>(str.str()), 0, str.length(), nullptr);
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
    inline bool stream_writer::write(double value)
    {
        string str = string_utils::double_to_str(value);

        return m_OutputStream->write(rm_const<char *>(str.str()), 0, str.length(), nullptr);
    }

}

#endif