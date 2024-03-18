#ifndef __DEEP_ENGINE_ZIP_READER_HPP__
#define __DEEP_ENGINE_ZIP_READER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/list.hpp"
#include "DE/io/stream.hpp"
#include "DE/core/string.hpp"

namespace deep
{

    class zip_reader
    {

        public:
            enum class compression_method
            {
                Unknown,
                Store,
                Deflate,
                Bzip2,
                Lzma,
                XZ,
                Zstd
            };

            enum class file_attribute : uint16_t
            {
                ReadOnly          = 0x0001,     // R
                Hidden            = 0x0002,     // H
                System            = 0x0004,     // S
                Directory         = 0x0008,     // D
                Archive           = 0x0010,     // A
                Compressed        = 0x0020,     // C
                Encrypted         = 0x0040,     // E
                Offline           = 0x0200,     // O
                SparseFile        = 0x0400,     // P
                NotContentIndexed = 0x0800,     // I
                Temporary         = 0x1000      // T
            };

            using next_entry_callback = bool (*)(const char *filename, int64_t compressedSize, int64_t uncompressedSize, compression_method method, file_attribute attributes, void *args);

        public:
            DE_API zip_reader();
            DE_API ~zip_reader();

            DE_API bool load(stream *inputStream);
            DE_API bool load(mem_ptr rawData, int32_t size);

            DE_API bool enumerate(next_entry_callback callback, void *args);
            DE_API bool extract_file(const char *filename, stream *outputStream);

            DE_API static string method_str(compression_method method);
            DE_API static string attributes_str(file_attribute attributes);

        private:
            mem_ptr m_RawData;
            void *m_ZipReader;

    };

}

#endif