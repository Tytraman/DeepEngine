#ifndef __DEEP_ENGINE_ZIP_WRITER_HPP__
#define __DEEP_ENGINE_ZIP_WRITER_HPP__

#include "DE/modules/zip/zip.hpp"

namespace deep
{

    class zip_writer : public zip
    {

        public:
            DE_API zip_writer(stream *outputStream);

            DE_API bool init();

            DE_API bool write_file(const char *filename, stream *is, int64_t *index = nullptr);

    };

}

#endif