#ifndef __DEEP_ENGINE_MAT_HPP__
#define __DEEP_ENGINE_MAT_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/vec.hpp>

#include <stdint.h>
#include <math.h>

namespace de {

	struct DE_API fmat2x2 {
		float x1;
		float x2;
		float y1;
		float y2;

		fmat2x2(float x1 = 1.0f, float y1 = 0.0f, float x2 = 0.0f, float y2 = 1.0f);

		fvec2 operator*(const fvec2 &vec) const;

		static fvec2 mul(const fmat2x2 &mat, const fvec2 &vec);

		static fvec2 scale(const fvec2 &vec, float scale1, float scale2);
		static fvec2 translate(const fvec2 &vec, float x, float y);
		static fvec2 rotate(const fvec2 &vec, float degrees);
	};

	namespace fmat3x3_index {
		enum t : uint8_t {
			x1 = 0, y1 = 1, z1 = 2,
			x2 = 3, y2 = 4, z2 = 5,
			x3 = 6, y3 = 7, z3 = 8
		};
	}

	struct DE_API fmat3x3 {
		float data[9];

		fmat3x3(
			float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f,
			float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f,
			float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f
		);

		fvec3 operator*(const fvec3 &vec) const;
		fmat3x3 operator*(const fmat3x3 &mat) const;
		float operator[](size_t index) const;

		float *ptr();

		static fvec3 mul(const fmat3x3 &mat, const fvec3 &vec);
		static fmat3x3 mul(const fmat3x3 &mat1, const fmat3x3 &mat2);

		static fmat3x3 translate(const fmat3x3 &mat, const fvec2 &vec);
		static fmat3x3 scale(const fmat3x3 &mat, const fvec2 &vec);
		static fmat3x3 rotate(const fmat3x3 &mat, float degrees);
	};

	/*
	==================
	fmat2x2::operator*
	==================
	*/
	inline fvec2 fmat2x2::operator*(const fvec2 &vec) const
	{
		return mul(*this, vec);
	}

	/*
	==================
	fmat3x3::operator*
	==================
	*/
	inline fvec3 fmat3x3::operator*(const fvec3 &vec) const
	{
		return mul(*this, vec);
	}

	/*
	==================
	fmat3x3::operator*
	==================
	*/
	inline fmat3x3 fmat3x3::operator*(const fmat3x3 &mat) const
	{
		return mul(*this, mat);
	}

	inline float fmat3x3::operator[](size_t index) const
	{
		return data[index];
	}

	inline float *fmat3x3::ptr()
	{
		return data;
	}

	/*
	============
	fmat2x2::mul
	============
	*/
	inline fvec2 fmat2x2::mul(const fmat2x2 &mat, const fvec2 &vec)
	{
		return fvec2::mul(vec, mat);
	}

	/*
	============
	fmat3x3::mul
	============
	*/
	inline fvec3 fmat3x3::mul(const fmat3x3 &mat, const fvec3 &vec)
	{
		return fvec3::mul(vec, mat);
	}

	/*
	============
	fmat3x3::mul
	============
	*/
	inline fmat3x3 fmat3x3::mul(const fmat3x3 &mat1, const fmat3x3 &mat2)
	{
		return {
			(mat1[fmat3x3_index::x1] * mat2[fmat3x3_index::x1]) + (mat1[fmat3x3_index::y1] * mat2[fmat3x3_index::x2]) + (mat1[fmat3x3_index::z1] * mat2[fmat3x3_index::x3]), (mat1[fmat3x3_index::x1] * mat2[fmat3x3_index::y1]) + (mat1[fmat3x3_index::y1] * mat2[fmat3x3_index::y2]) + (mat1[fmat3x3_index::z1] * mat2[fmat3x3_index::y3]), (mat1[fmat3x3_index::x1] * mat2[fmat3x3_index::z1]) + (mat1[fmat3x3_index::y1] * mat2[fmat3x3_index::z2]) + (mat1[fmat3x3_index::z1] * mat2[fmat3x3_index::z3]),
			(mat1[fmat3x3_index::x2] * mat2[fmat3x3_index::x1]) + (mat1[fmat3x3_index::y2] * mat2[fmat3x3_index::x2]) + (mat1[fmat3x3_index::z2] * mat2[fmat3x3_index::x3]), (mat1[fmat3x3_index::x2] * mat2[fmat3x3_index::y1]) + (mat1[fmat3x3_index::y2] * mat2[fmat3x3_index::y2]) + (mat1[fmat3x3_index::z2] * mat2[fmat3x3_index::y3]), (mat1[fmat3x3_index::x2] * mat2[fmat3x3_index::z1]) + (mat1[fmat3x3_index::y2] * mat2[fmat3x3_index::z2]) + (mat1[fmat3x3_index::z2] * mat2[fmat3x3_index::z3]),
			(mat1[fmat3x3_index::x3] * mat2[fmat3x3_index::x1]) + (mat1[fmat3x3_index::y3] * mat2[fmat3x3_index::x2]) + (mat1[fmat3x3_index::z3] * mat2[fmat3x3_index::x3]), (mat1[fmat3x3_index::x3] * mat2[fmat3x3_index::y1]) + (mat1[fmat3x3_index::y3] * mat2[fmat3x3_index::y2]) + (mat1[fmat3x3_index::z3] * mat2[fmat3x3_index::y3]), (mat1[fmat3x3_index::x3] * mat2[fmat3x3_index::z1]) + (mat1[fmat3x3_index::y3] * mat2[fmat3x3_index::z2]) + (mat1[fmat3x3_index::z3] * mat2[fmat3x3_index::z3])
		};
	}

	/*
	==============
	fmat2x2::scale
	==============
	*/
	inline fvec2 fmat2x2::scale(const fvec2 &vec, float scale1, float scale2)
	{
		return fvec2::mul(vec, { scale1, 0.0f, 0.0f, scale2 } );
	}

	inline fvec2 fmat2x2::translate(const fvec2 &vec, float x, float y)
	{
		return { vec.x + x, vec.y + y };
	}

	/*
	===============
	fmat2x2::rotate
	===============
	*/
	inline fvec2 fmat2x2::rotate(const fvec2 &vec, float degrees)
	{
		float rad = DE_DEG_RAD(degrees);
		return fvec2::mul(vec, {
			cosf(rad), -sinf(rad),
			sinf(rad), cosf(rad) 
		});
	}

	/*
	==================
	fmat3x3::translate
	==================
	*/
	inline fmat3x3 fmat3x3::translate(const fmat3x3 &mat, const fvec2 &vec)
	{
		return mul(
			mat,
			{
				1.0f, 0.0f, vec.x,
				0.0f, 1.0f, vec.y,
				0.0f, 0.0f, 1.0f
			}
		);
	}

	/*
	==============
	fmat3x3::scale
	==============
	*/
	inline fmat3x3 fmat3x3::scale(const fmat3x3 &mat, const fvec2 &vec)
	{
		return mul(
			mat,
			{
				vec.x, 0.0f,  0.0f,
				0.0f,  vec.y, 0.0f,
				0.0f,  0.0f,  1.0f
			}
		);
	}

	/*
	===============
	fmat3x3::rotate
	===============
	*/
	inline fmat3x3 fmat3x3::rotate(const fmat3x3 &mat, float degrees)
	{
		float rad = DE_DEG_RAD(degrees);
		return mul(
			mat,
			{
				cosf(rad), -sinf(rad), 0.0f,
				sinf(rad),  cosf(rad), 0.0f,
				0.0f,       0.0f,      1.0f
			}
		);
	}

}

#endif