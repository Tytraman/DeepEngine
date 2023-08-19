#ifndef __DEEP_ENGINE_IMAGE_HPP__
#define __DEEP_ENGINE_IMAGE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

#include <png.h>

namespace de {

	namespace ImageColorType {
		enum t : int {
			None    = -1,
			Gray    = PNG_COLOR_TYPE_GRAY,
			Palette = PNG_COLOR_TYPE_PALETTE,
			RGB     = PNG_COLOR_TYPE_RGB,
			RGBA    = PNG_COLOR_TYPE_RGBA,
			GA      = PNG_COLOR_TYPE_GA
		};
	}

	namespace ImageChannel {
		enum t : uint8_t {
			Red,
			Green,
			Blue,
			Alpha
		};
	}

	constexpr uint32_t ImageColorSpaceSRGB = (('B' << 24) | ('G' << 16) | ('R' << 8) | ('s'));

}


#endif