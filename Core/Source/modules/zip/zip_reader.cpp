#include "DE/modules/zip/zip_reader.hpp"

#include <zlib/contrib/minizip/unzip.h>

namespace deep
{

    /*
    ======================
    zip_reader::zip_reader
    ======================
    */
    zip_reader::zip_reader(stream *inputStream)
        : m_InputStream(inputStream)
    { }

    bool zip_reader::uncompress()
    {
        if(!m_InputStream->can_read())
        {
            return false;
        }

        if(!m_InputStream->is_opened())
        {
            if(!m_InputStream->open())
            {
                return false;
            }
        }

        size_t len = m_InputStream->get_length() - m_InputStream->get_position();
        if(len == 0)
        {
            return false;
        }

        return true;
    }

}
