#include "modules/zip/zip_reader.hpp"

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

        zip_source_t *source = static_cast<zip_source_t *>(create_source_function(&m_Context));

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

    

}
