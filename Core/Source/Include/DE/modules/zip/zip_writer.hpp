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

    };

}

#endif