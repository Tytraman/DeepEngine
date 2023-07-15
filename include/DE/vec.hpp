#ifndef __DEEP_ENGINE_VEC_HPP__
#define __DEEP_ENGINE_VEC_HPP__

#include <DE/def.h>
#include <DE/mat.hpp>

#include <stdint.h>
#include <math.h>

namespace de {

	class Renderer;

	struct DE_API vec2 {
		int32_t x;
		int32_t y;

		vec2(int32_t x = 1, int32_t y = 1);

		void draw(Renderer &renderer) const;
	};

	/*
	==========
	vec2::vec2
	==========
	*/
	inline vec2::vec2(int32_t _x, int32_t _y)
		: x(_x), y(_y)
	{ }

	struct DE_API fvec2 {
		float x;
		float y;

		fvec2(float x = 1.0f, float y = 1.0f);

		void draw(Renderer &renderer) const;

		fvec2 operator+(const fvec2 &vec) const;
		fvec2 operator+(const vec2 &vec) const;
		fvec2 operator+(float valueToAdd) const;
		fvec2 operator-(const fvec2 &vec) const;
		fvec2 operator-(float valueToSub) const;
		fvec2 operator*(float scalar) const;
		fvec2 operator*(const fmat2x2 &mat) const;
		void operator*=(float scalar);

		static fvec2 add(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 add(const fvec2 &vec1, const vec2 &vec2);
		static fvec2 add(const fvec2 &vec, float valueToAdd);
		static fvec2 sub(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 sub(const fvec2 &vec, float valueToSub);
		static fvec2 scale(const fvec2 &vec, float scalar);
		static float dot(const fvec2 &vec1, const fvec2 &vec2);
		static float magn(const fvec2 &vec);
		static float angle(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 mul(const fvec2 &vec, const fmat2x2 &mat);
		static fvec2 inv(const fvec2 &vec);
		static fvec2 interpolateX(const fvec2 &vec1, const fvec2 &vec2, float y);
		static fvec2 interpolateY(const fvec2 &vec1, const fvec2 &vec2, float x);
	};

	/*
	============
	fvec2::fvec2
	============
	*/
	inline fvec2::fvec2(float _x, float _y)
		: x(_x), y(_y)
	{ }

	/*
	================
	fvec2::operator+
	================
	*/
	inline fvec2 fvec2::operator+(const fvec2 &vec) const
	{
		return add(*this, vec);
	}

	/*
	================
	fvec2::operator+
	================
	*/
	inline fvec2 fvec2::operator+(const vec2 &vec) const
	{
		return add(*this, vec);
	}

	/*
	================
	fvec2::operator+
	================
	*/
	inline fvec2 fvec2::operator+(float valueToAdd) const
	{
		return add(*this, valueToAdd);
	}

	/*
	================
	fvec2::operator-
	================
	*/
	inline fvec2 fvec2::operator-(const fvec2 &vec) const
	{
		return sub(*this, vec);
	}

	/*
	================
	fvec2::operator-
	================
	*/
	inline fvec2 fvec2::operator-(float valueToSub) const
	{
		return sub(*this, valueToSub);
	}

	/*
	================
	fvec2::operator*
	================
	*/
	inline fvec2 fvec2::operator*(float scalar) const
	{
		return scale(*this, scalar);
	}

	/*
	================
	fvec2::operator*
	================
	*/
	inline fvec2 fvec2::operator*(const fmat2x2 &mat) const
	{
		return mul(*this, mat);
	}

	/*
	=================
	fvec2::operator*=
	=================
	*/
	inline void fvec2::operator*=(float scalar)
	{
		*this = scale(*this, scalar);
	}

	/*
	==========
	fvec2::add
	==========
	*/
	inline fvec2 fvec2::add(const fvec2 &vec1, const fvec2 &vec2)
	{
		return {
			vec1.x + vec2.x,
			vec1.y + vec2.y
		};
	}

	/*
	==========
	fvec2::add
	==========
	*/
	inline fvec2 fvec2::add(const fvec2 &vec1, const vec2 &vec2)
	{
		return {
			vec1.x + vec2.x,
			vec1.y + vec2.y
		};
	}

	/*
	==========
	fvec2::add
	==========
	*/
	inline fvec2 fvec2::add(const fvec2 &vec, float valueToAdd)
	{
		return {
			vec.x + valueToAdd,
			vec.y + valueToAdd
		};
	}

	/*
	==========
	fvec2::sub
	==========
	*/
	inline fvec2 fvec2::sub(const fvec2 &vec1, const fvec2 &vec2)
	{
		return {
			vec1.x - vec2.x,
			vec1.y - vec2.y
		};
	}

	/*
	==========
	fvec2::sub
	==========
	*/
	inline fvec2 fvec2::sub(const fvec2 &vec, float valueToSub)
	{
		return {
			vec.x - valueToSub,
			vec.y - valueToSub
		};
	}

	/*
	============
	fvec2::scale
	============
	*/
	inline fvec2 fvec2::scale(const fvec2 &vec, float scalar)
	{
		return {
			vec.x * scalar,
			vec.y * scalar
		};
	}

	/*
	==========
	fvec2::dot
	==========
	*/
	inline float fvec2::dot(const fvec2 &vec1, const fvec2 &vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	/*
	===========
	fvec2::magn
	===========
	*/
	inline float fvec2::magn(const fvec2 &vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y);
	}

	/*
	============
	fvec2::angle
	============
	*/
	inline float fvec2::angle(const fvec2 &vec1, const fvec2 &vec2)
	{
		return acosf(dot(vec1, vec2) / (magn(vec1) * magn(vec2))) * (float) (180.0f / M_PI);
	}

	/*
	==========
	fvec2::mul
	==========
	*/
	inline fvec2 fvec2::mul(const fvec2 &vec, const fmat2x2 &mat)
	{
		return {
			vec.x * mat.x1 + vec.y * mat.x2,
			vec.x * mat.y1 + vec.y * mat.y2
		};
	}

	/*
	==========
	fvec2::inv
	==========
	*/
	inline fvec2 fvec2::inv(const fvec2 &vec)
	{
		return scale(vec, -1.0f);
	}

	/*
	===================
	fvec2::interpolateX
	===================
	*/
	inline fvec2 fvec2::interpolateX(const fvec2 &vec1, const fvec2 &vec2, float y)
	{
		return {
			vec1.x * (vec2.y - y) / (vec2.y - vec1.y) +
			vec2.x * (y - vec1.y) / (vec2.y - vec1.y),
			y
		};
	}

	/*
	===================
	fvec2::interpolateY
	===================
	*/
	inline fvec2 fvec2::interpolateY(const fvec2 &vec1, const fvec2 &vec2, float x)
	{
		return {
			vec1.y * (vec2.x - x) / (vec2.x - vec1.x) +
			vec2.y * (x - vec1.x) / (vec2.x - vec1.x),
			x
		};
	}

}

#endif