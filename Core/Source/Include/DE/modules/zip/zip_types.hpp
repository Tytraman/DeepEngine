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

}

#endif