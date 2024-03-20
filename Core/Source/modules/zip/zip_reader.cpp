#include "DE/modules/zip/zip_reader.hpp"

#include <zip.h>

namespace deep
{

    /*
    ======================
    zip_reader::zip_reader
    ======================
    */
    zip_reader::zip_reader(stream *inputStream)
        : zip(inputStream)
    {  }

    /*
    ================
    zip_reader::init
    ================
    */
    bool zip_reader::init()
    {
        zip_error_t err;
        zip_t *zip;

        zip_source_t *source = static_cast<zip_source_t *>(create_source_function());

        if(source == nullptr)
        {
            return false;
        }

        zip = zip_open_from_source(source, ZIP_RDONLY, &err);

        if(zip == nullptr)
        {
            zip_source_close(source);
            zip_source_free(source);

            return false;
        }

        m_Zip = zip;
        m_ZipSource = source;

        return true;
    }

    /*
    =====================
    zip_reader::read_file
    =====================
    */
    bool zip_reader::read_file(int64_t index, stream *os)
    {
        ref<stream> strm = os;
        zip_t *zip = static_cast<zip_t *>(m_Zip);
        zip_file_t *file;
        bool ret = true;

        if(!strm->can_write())
        {
            return false;
        }

        if(!strm->is_opened())
        {
            if(!strm->open())
            {
                return false;
            }
        }

        file = zip_fopen_index(zip, static_cast<zip_uint64_t>(index), 0);
        if(file == nullptr)
        {
            return false;
        }

        uint8_t buffer[4096];
        zip_int64_t bytesRead;

        while(true)
        {
            bytesRead = zip_fread(file, buffer, sizeof(buffer));

            if(bytesRead == 0)
            {
                break;
            }

            if(bytesRead == -1)
            {
                ret = false;
                break;
            }

            if(!strm->write(buffer, 0, static_cast<size_t>(bytesRead), nullptr))
            {
                ret = false;
                break;
            }
        }

        zip_fclose(file);

        return ret;
    }

}
