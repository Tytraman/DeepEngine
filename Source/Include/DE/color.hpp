#ifndef __DEEP_ENGINE_COLOR_HPP__
#define __DEEP_ENGINE_COLOR_HPP__

#include <DE/def.hpp>

#include <stdint.h>

namespace deep
{

	struct DE_API color
    {
		uint8_t R;
		uint8_t G;
		uint8_t B;

		color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
	};

	struct DE_API colora
    {
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;

		colora(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0);
	};

}

#endif