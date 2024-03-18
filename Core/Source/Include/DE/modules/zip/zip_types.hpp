#ifndef __DEEP_ENGINE_ZIP_TYPES_HPP__
#define __DEEP_ENGINE_ZIP_TYPES_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"

namespace deep
{

    enum class zip_compression_method
    {
        Unknown,
        Store,
        Deflate,
        Bzip2,
        Lzma,
        XZ,
        Zstd
    };

    enum class zip_file_attribute : uint16_t
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

}

#endif