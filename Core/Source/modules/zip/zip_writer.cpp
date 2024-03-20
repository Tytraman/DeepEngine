#include "DE/modules/zip/zip_writer.hpp"

#include <zip.h>

namespace deep
{

    /*
    ======================
    zip_writer::zip_writer
    ======================
    */
    zip_writer::zip_writer(stream *outputStream)
        : zip(outputStream)
    { }

    /*
    ================
    zip_writer::init
    ================
    */
    bool zip_writer::init()
    {
        zip_error_t err;
        zip_t *zip;

        zip_source_t *source = static_cast<zip_source_t *>(create_source_function(&m_Context));

        if(source == nullptr)
        {
            return false;
        }

        zip = zip_open_from_source(source, ZIP_CREATE, &err);

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
    ======================
    zip_writer::write_file
    ======================
    */
    bool zip_writer::write_file(const char *filename, stream *is, int64_t *index)
    {
        ref<stream> strm = is;
        zip_t *zip = static_cast<zip_t *>(m_Zip);
        zip_source_t *source;
        zip_error_t *err;
        bool ret = true;

        if(!strm->can_read())
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

        size_t len = strm->get_length() - strm->get_position();
        mem_ptr buffer = mem::alloc(len);
        size_t bytesRead = 0;

        strm->read(buffer, 0, len, &bytesRead);

        source = zip_source_buffer(zip, buffer, bytesRead, 0);

        if(source == nullptr)
        {
            mem::free(buffer);
            return false;
        }

        zip_int64_t idx = zip_file_add(zip, filename, source, ZIP_FL_OVERWRITE);

        if(idx == -1)
        {
            err = zip_get_error(zip);

            zip_source_free(source);

            return false;
        }

        if(index != nullptr)
        {
            *index = static_cast<int64_t>(idx);
        }

        /*if(zip_set_file_compression(zip, idx, ZIP_CM_DEFLATE, 9) == -1)
        {
            err = zip_get_error(zip);

            zip_source_close(source);

            return false;
        }

        if(zip_file_set_external_attributes(zip, idx, 0, ZIP_OPSYS_VFAT, 0) == -1)
        {
            err = zip_get_error(zip);

            zip_source_close(source);

            return false;
        }*/

        return true;
    }

}
