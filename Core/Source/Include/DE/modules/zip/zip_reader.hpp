#ifndef __DEEP_ENGINE_ZIP_READER_HPP__
#define __DEEP_ENGINE_ZIP_READER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/list.hpp"
#include "DE/io/stream.hpp"

namespace deep
{

#define DE_ZIP_BUFFER_SIZE 16384

    class zip_reader
    {

        public:
            DE_API zip_reader(stream *inputStream);

            DE_API bool uncompress();

        private:
            ref<stream> m_InputStream;
            list<uint8_t> m_Data;

    };

}

#endif