#ifndef __DEEP_ENGINE_MEMORY_STREAM_HPP__
#define __DEEP_ENGINE_MEMORY_STREAM_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/io/stream.hpp"
#include "DE/core/list.hpp"

namespace deep
{

    class memory_stream : public stream
    {

        public:
            DE_API memory_stream();

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

        protected:
            list<uint8_t> m_Data;
            size_t m_Position;

    };

    /*
    ===================
    memory_stream::open
    ===================
    */
    inline bool memory_stream::open()
    {
        return true;
    }

    /*
    ====================
    memory_stream::close
    ====================
    */
    inline bool memory_stream::close()
    {
        m_Data.free();
        m_Position = 0;

        return true;
    }

    /*
    ====================
    memory_stream::flush
    ====================
    */
    inline bool memory_stream::flush()
    {
        return true;
    }

    /*
    =======================
    memory_stream::can_read
    =======================
    */
    inline bool memory_stream::can_read() const
    {
        return true;
    }

    /*
    ========================
    memory_stream::can_write
    ========================
    */
    inline bool memory_stream::can_write() const
    {
        return true;
    }

    /*
    ==========================
    memory_stream::can_timeout
    ==========================
    */
    inline bool memory_stream::can_timeout() const
    {
        return false;
    }

    /*
    =======================
    memory_stream::can_seek
    =======================
    */
    inline bool memory_stream::can_seek() const
    {
        return true;
    }

    /*
    =========================
    memory_stream::get_length
    =========================
    */
    inline size_t memory_stream::get_length() const
    {
        return m_Data.count();
    }

    /*
    =========================
    memory_stream::set_length
    =========================
    */
    inline bool memory_stream::set_length(size_t length)
    {
        return m_Data.reserve(length);
    }

    /*
    ===========================
    memory_stream::get_position
    ===========================
    */
    inline size_t memory_stream::get_position() const
    {
        return m_Position;
    }

    /*
    ===============================
    memory_stream::get_read_timeout
    ===============================
    */
    inline uint32_t memory_stream::get_read_timeout() const
    {
        return 0;
    }

    /*
    ================================
    memory_stream::get_write_timeout
    ================================
    */
    inline uint32_t memory_stream::get_write_timeout() const
    {
        return 0;
    }

    /*
    ===============================
    memory_stream::set_read_timeout
    ===============================
    */
    inline void memory_stream::set_read_timeout(uint32_t /*value*/)
    { }

    /*
    ================================
    memory_stream::set_write_timeout
    ================================
    */
    inline void memory_stream::set_write_timeout(uint32_t /*value*/)
    { }

    /*
    ========================
    memory_stream::is_opened
    ========================
    */
    inline bool memory_stream::is_opened() const
    {
        return true;
    }

}


#endif