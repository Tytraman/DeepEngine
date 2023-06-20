#ifndef __DEEP_ENGINE_MAT_HPP__
#define __DEEP_ENGINE_MAT_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stdint.h>
#include <math.h>

namespace de {

	struct DE_API fmat2x2 {
		float x1;
		float x2;
		float y1;
		float y2;

		fmat2x2(float x1 = 1.0f, float y1 = 1.0f, float x2 = 1.0f, float y2 = 1.0f);

		static fmat2x2 rotate(float degrees);
	};

	/*
	================
	fmat2x2::fmat2x2
	================
	*/
	inline fmat2x2::fmat2x2(float _x1, float _y1, float _x2, float _y2)
		: x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	{ }

	/*
	===============
	fmat2x2::rotate
	===============
	*/
	inline fmat2x2 fmat2x2::rotate(float degrees)
	{
		float rad = DE_DEG_RAD(degrees);
		return {
			cosf(rad), -sinf(rad),
			sinf(rad), cosf(rad) 
		};
	}

}

#endif