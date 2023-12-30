#ifndef __DEEP_ENGINE_IMAGE_HPP__
#define __DEEP_ENGINE_IMAGE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

#include <png.h>

namespace deep
{

    enum class image_color_space : int
    {
        None    = -1,
        Gray,
        Palette,
        RGB,
        RGBA,
        GA,
        BGR,
        BGRA
    };

    enum class image_channel : uint8_t
    {
        Red,
        Green,
        Blue,
        Alpha
    };

    constexpr uint32_t ImageColorSpaceSRGB = (('B' << 24) | ('G' << 16) | ('R' << 8) | ('s'));

}


#endif