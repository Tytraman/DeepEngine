#ifndef __DEEP_ENGINE_ZIP_READER_HPP__
#define __DEEP_ENGINE_ZIP_READER_HPP__

#include "modules/zip/zip.hpp"

namespace deep
{

    class zip_reader : public zip
    {

        public:
            DE_API zip_reader(stream *inputStream);

            DE_API bool init();

            DE_API bool read_file(int64_t index, stream *os);

    };

}

#endif