#include "DE/io/memory_stream.hpp"

namespace deep
{

    /*
    ============================
    memory_stream::memory_stream
    ============================
    */
    memory_stream::memory_stream()
        : m_Data(256),
          m_Position(0),
          stream()
    { }

    /*
    ===================
    memory_stream::seek
    ===================
    */
    size_t memory_stream::seek(ssize_t offset, seek_origin origin)
    {
        switch(origin)
        {
            default: break;
            case seek_origin::Begin:
            {
                if(offset <= 0)
                {
                    m_Position = 0;
                }
                else
                {
                    m_Position = offset;
                }
            } break;
            case seek_origin::Current:
            {
                if(offset < 0)
                {
                    if(static_cast<size_t>(std::abs(offset)) >= m_Position)
                    {
                        m_Position = 0;

                        break;
                    }
                }

                m_Position += offset;
            } break;
            case seek_origin::End:
            {
                size_t position = get_length();

                if(offset < 0)
                {
                    if(static_cast<size_t>(std::abs(offset)) >= position)
                    {
                        m_Position = 0;

                        break;
                    }
                }

                m_Position = position + offset;
            } break;
        }

        return m_Position;
    }

    /*
    ===================
    memory_stream::read
    ===================
    */
    bool memory_stream::read(mem_ptr buffer, size_t offset, size_t count, size_t *bytesRead)
    {
        size_t position = m_Position;
        size_t length = get_length();
        size_t targetPosition;
        
        if(position >= length)
        {
            if(bytesRead != nullptr)
            {
                *bytesRead = 0;
            }

            return true;
        }

        targetPosition = position + count;

        if(targetPosition > length)
        {
            count -= targetPosition - length;
        }

        memcpy(static_cast<uint8_t *>(buffer) + offset, &m_Data[position], count);

        m_Position += count;

        if(bytesRead != nullptr)
        {
            *bytesRead = count;
        }

        return true;
    }

    /*
    ====================
    memory_stream::write
    ====================
    */
    bool memory_stream::write(mem_ptr buffer, size_t offset, size_t count, size_t *bytesWrite)
    {
        uint8_t *buff = static_cast<uint8_t *>(buffer);
        size_t position = m_Position;
        size_t length = get_length();

        size_t bw = 0;

        if(position > length)
        {
            bw = position - length;

            if(!m_Data.set(static_cast<uint8_t>(0), length, bw))
            {
                return false;
            }
        }

        if(!m_Data.set(buff + offset, position, count))
        {
            return false;
        }

        bw += count;

        if(bytesWrite != nullptr)
        {
            *bytesWrite = bw;
        }

        m_Position += count;

        return true;
    }

    /*
    ======================
    memory_stream::copy_to
    ======================
    */
    bool memory_stream::copy_to(stream &other)
    {
        size_t position = m_Position;
        size_t length = get_length();
        size_t diff;

        // Il ne reste plus rien à lire.
        if(position >= length)
        {
            return true;
        }

        diff = length - position;

        bool ret = other.write(&m_Data[position], 0, diff, nullptr);

        m_Position = length;

        return ret;
    }

}
