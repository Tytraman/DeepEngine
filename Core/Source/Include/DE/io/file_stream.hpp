#ifndef __DEEP_ENGINE_FILE_STREAM_HPP__
#define __DEEP_ENGINE_FILE_STREAM_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/string.hpp"
#include "DE/io/stream.hpp"

namespace deep
{

    class file_stream : public stream
    {

        public:
            enum class file_mode
            {
                None,
                Append,
                Create,
                CreateNew,
                Open,
                OpenOrCreate,
                Truncate
            };

            enum class file_access
            {
                None,
                Read,
                Write,
                ReadWrite
            };

            enum class file_share
            {
                None,
                Delete,
                Read,
                Write,
                ReadWrite
            };

        public:
            DE_API file_stream(const char *filename, file_mode mode, file_access access, file_share share);
            DE_API file_stream(const char *filename, os_fd fd);
            DE_API ~file_stream();

            DE_API virtual bool open() override;
            DE_API virtual bool close() override;
            DE_API virtual bool flush() override;

            DE_API virtual bool can_read() const override;
            DE_API virtual bool can_write() const override;
            DE_API virtual bool can_timeout() const override;
            DE_API virtual bool can_seek() const override;

            DE_API virtual size_t get_length() const override;
            DE_API virtual bool set_length(size_t length) override;

            DE_API virtual size_t get_position() const override;

            DE_API virtual uint32_t get_read_timeout() const override;
            DE_API virtual uint32_t get_write_timeout() const override;

            DE_API virtual void set_read_timeout(uint32_t value) override;
            DE_API virtual void set_write_timeout(uint32_t value) override;

            DE_API virtual size_t seek(ssize_t offset, seek_origin origin) override;

            DE_API virtual bool read(mem_ptr buffer, size_t offset, size_t count, size_t *bytesRead = nullptr) override;
            DE_API virtual bool write(mem_ptr buffer, size_t offset, size_t count, size_t *bytesWrite = nullptr) override;

            DE_API virtual bool copy_to(stream &other) override;

            DE_API virtual bool is_opened() const override;

            DE_API string get_filename() const;

        protected:
            string m_Filename;
            file_mode m_Mode;
            file_access m_Access;
            file_share m_Share;
            os_fd m_FD;
            bool m_IsOpened;

    };

    /*
    ==================
    file_stream::flush
    ==================
    */
    inline bool file_stream::flush()
    {
#if DE_WINDOWS

        return FlushFileBuffers(m_FD);

#else
#error Need implementation
#endif
    }

    /*
    =====================
    file_stream::can_read
    =====================
    */
    inline bool file_stream::can_read() const
    {
        return
            m_Access == file_access::Read ||
            m_Access == file_access::ReadWrite;
    }

    /*
    ======================
    file_stream::can_write
    ======================
    */
    inline bool file_stream::can_write() const
    {
        return
            m_Access == file_access::Write ||
            m_Access == file_access::ReadWrite;
    }

    /*
    ========================
    file_stream::can_timeout
    ========================
    */
    inline bool file_stream::can_timeout() const
    {
        return false;
    }

    /*
    =====================
    file_stream::can_seek
    =====================
    */
    inline bool file_stream::can_seek() const
    {
        return true;
    }

    /*
    =========================
    file_stream::get_position
    =========================
    */
    inline size_t file_stream::get_position() const
    {
#if DE_WINDOWS

        LARGE_INTEGER linteger;
        linteger.QuadPart = 0;

        if(!SetFilePointerEx(m_FD, linteger, &linteger, FILE_CURRENT))
        {
            return 0;
        }

        return linteger.QuadPart;

#else
#error Need implementation
#endif
    }

    /*
    =============================
    file_stream::get_read_timeout
    =============================
    */
    inline uint32_t file_stream::get_read_timeout() const
    {
        return 0;
    }

    /*
    ==============================
    file_stream::get_write_timeout
    ==============================
    */
    inline uint32_t file_stream::get_write_timeout() const
    {
        return 0;
    }

    /*
    =============================
    file_stream::set_read_timeout
    =============================
    */
    inline void file_stream::set_read_timeout(uint32_t /*value*/)
    { }

    /*
    ==============================
    file_stream::set_write_timeout
    ==============================
    */
    inline void file_stream::set_write_timeout(uint32_t /*value*/)
    { }

    /*
    =================
    file_stream::seek
    =================
    */
    inline size_t file_stream::seek(ssize_t offset, seek_origin origin)
    {
#if DE_WINDOWS

        LARGE_INTEGER linteger;
        linteger.QuadPart = offset;

        DWORD method;
        switch(origin)
        {
            default:
            {
                linteger.QuadPart = 0;
                method = FILE_CURRENT;
            } break;
            case seek_origin::Begin:
            {
                method = FILE_BEGIN;
            } break;
            case seek_origin::Current:
            {
                method = FILE_CURRENT;
            } break;
            case seek_origin::End:
            {
                method = FILE_END;
            } break;
        }

        SetFilePointerEx(m_FD, linteger, &linteger, method);

        return linteger.QuadPart;

#else
#error Need implementation
#endif
    }

    /*
    ======================
    file_stream::is_opened
    ======================
    */
    inline bool file_stream::is_opened() const
    {
        return m_IsOpened;
    }

    /*
    =========================
    file_stream::get_filename
    =========================
    */
    inline string file_stream::get_filename() const
    {
        return m_Filename;
    }

}


#endif