#ifndef __DEEP_ENGINE_ZIP_READER_HPP__
#define __DEEP_ENGINE_ZIP_READER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/io/stream.hpp"
#include "DE/core/string.hpp"
#include "DE/modules/zip/zip_types.hpp"

namespace deep
{

    class zip_reader
    {

        public:

            using next_entry_callback = bool (*)(const char *filename, int64_t compressedSize, int64_t uncompressedSize, zip_compression_method method, zip_file_attribute attributes, void *args);

        public:
            DE_API zip_reader();
            DE_API ~zip_reader();

            DE_API bool load(stream *inputStream);
            DE_API bool load(mem_ptr rawData, int32_t size);

            DE_API bool enumerate(next_entry_callback callback, void *args);
            DE_API bool extract_file(const char *filename, stream *outputStream);

            DE_API static string method_str(zip_compression_method method);
            DE_API static string attributes_str(zip_file_attribute attributes);

        private:
            mem_ptr m_RawData;
            void *m_ZipReader;

    };

}

#endif