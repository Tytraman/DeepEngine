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

        zip_source_t *source = static_cast<zip_source_t *>(create_source_function());

        if(source == nullptr)
        {
            return false;
        }

        zip = zip_open_from_source(source, 0, &err);

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

}
