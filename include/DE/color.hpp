#ifndef __DEEP_ENGINE_COLOR_HPP__
#define __DEEP_ENGINE_COLOR_HPP__

#include <DE/def.h>
#include <DE/int.hpp>

namespace de {

	struct DE_API color {
		uint8 R;
		uint8 G;
		uint8 B;

		color(uint8 r = 0, uint8 g = 0, uint8 b = 0);
	};

	struct DE_API colora {
		uint8 R;
		uint8 G;
		uint8 B;
		uint8 A;

		colora(uint8 r = 0, uint8 g = 0, uint8 b = 0, uint8 a = 0);
	};

}

#endif