#ifndef __DEEP_ENGINE_VEC_IMP_HPP__
#define __DEEP_ENGINE_VEC_IMP_HPP__

#include <DE/vec.hpp>
#include <DE/mat.hpp>

#include <math.h>

namespace de {

	/*
	============
	fvec2::fvec2
	============
	*/
	inline fvec2::fvec2(float _x, float _y)
		: x(_x), y(_y)
	{ }

	/*
	============
	fvec3::fvec3
	============
	*/
	inline fvec3::fvec3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
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
	fvec3::operator+
	================
	*/
	inline fvec3 fvec3::operator+(const fvec3 &vec) const
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
	fvec3::operator+
	================
	*/
	inline fvec3 fvec3::operator+(float valueToAdd) const
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
	fvec3::operator-
	================
	*/
	inline fvec3 fvec3::operator-(const fvec3 &vec) const
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
	fvec3::operator-
	================
	*/
	inline fvec3 fvec3::operator-(float valueToSub) const
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
	fvec3::operator*
	================
	*/
	inline fvec3 fvec3::operator*(float scalar) const
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
	================
	fvec3::operator*
	================
	*/
	inline fvec3 fvec3::operator*(const fmat3x3 &mat) const
	{
		return mul(*this, mat);
	}

	/*
	=================
	fvec2::operator-=
	=================
	*/
	inline void fvec2::operator-=(float value)
	{
		*this = sub(*this, value);
	}

	/*
	=================
	fvec2::operator+=
	=================
	*/
	inline void fvec2::operator+=(const fvec2 &v)
	{
		*this = add(*this, v);
	}

	/*
	=================
	fvec2::operator+=
	=================
	*/
	inline void fvec2::operator+=(float value)
	{
		*this = add(*this, value);
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
	=================
	fvec3::operator*=
	=================
	*/
	inline void fvec3::operator*=(float scalar)
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
	fvec3::add
	==========
	*/
	inline fvec3 fvec3::add(const fvec3 &vec1, const fvec3 &vec2)
	{
		return {
			vec1.x + vec2.x,
			vec1.y + vec2.y,
			vec1.z + vec2.z
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
	fvec3::add
	==========
	*/
	inline fvec3 fvec3::add(const fvec3 &vec, float valueToAdd)
	{
		return {
			vec.x + valueToAdd,
			vec.y + valueToAdd,
			vec.z + valueToAdd
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
	fvec3::sub
	==========
	*/
	inline fvec3 fvec3::sub(const fvec3 &vec1, const fvec3 &vec2)
	{
		return {
			vec1.x - vec2.x,
			vec1.y - vec2.y,
			vec1.z - vec2.z
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
	==========
	fvec3::sub
	==========
	*/
	inline fvec3 fvec3::sub(const fvec3 &vec, float valueToSub)
	{
		return {
			vec.x - valueToSub,
			vec.y - valueToSub,
			vec.z - valueToSub
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
	============
	fvec3::scale
	============
	*/
	inline fvec3 fvec3::scale(const fvec3 &vec, float scalar)
	{
		return {
			vec.x * scalar,
			vec.y * scalar,
			vec.z * scalar
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
	==========
	fvec3::dot
	==========
	*/
	inline float fvec3::dot(const fvec3 &vec1, const fvec3 &vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
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
	===========
	fvec3::magn
	===========
	*/
	inline float fvec3::magn(const fvec3 &vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	/*
	===========
	fvec2::norm
	===========
	*/
	inline fvec2 fvec2::norm(const fvec2 &v)
	{
		float length = magn(v);
		return {
			v.x / length,
			v.y / length
		};
	}

	/*
	============
	fvec2::angle
	============
	*/
	inline float fvec2::angle(const fvec2 &vec1, const fvec2 &vec2)
	{
		return acosf(dot(vec1, vec2) / (magn(vec1) * magn(vec2))) * (float) (180.0f / M_PI);	// acosf retourne un résultat en radians, on la convertie donc en degrées.
	}

	/*
	============
	fvec3::angle
	============
	*/
	inline float fvec3::angle(const fvec3 &vec1, const fvec3 &vec2)
	{
		return acosf(dot(vec1, vec2) / (magn(vec1) * magn(vec2))) * (float) (180.0f / M_PI);	// acosf retourne un résultat en radians, on la convertie donc en degrées.
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
	fvec3::mul
	==========
	*/
	inline fvec3 fvec3::mul(const fvec3 &vec, const fmat3x3 &mat)
	{
		return {
			(vec.x * mat.x1) + (vec.y * mat.y1) + (vec.z * mat.z1),
			(vec.x * mat.x2) + (vec.y * mat.y2) + (vec.z * mat.z2),
			(vec.x * mat.x3) + (vec.y * mat.y3) + (vec.z * mat.z3)
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
	==========
	fvec3::inv
	==========
	*/
	inline fvec3 fvec3::inv(const fvec3 &vec)
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