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

	struct DE_API fmat3x3 {
		float x1;
		float x2;
		float x3;
		float y1;
		float y2;
		float y3;
		float z1;
		float z2;
		float z3;

		fmat3x3(
			float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f,
			float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f,
			float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f
		);

		fvec3 operator*(const fvec3 &vec) const;
		fmat3x3 operator*(const fmat3x3 &mat) const;

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
			(mat1.x1 * mat2.x1) + (mat1.y1 * mat2.x2) + (mat1.z1 * mat2.x3), (mat1.x1 * mat2.y1) + (mat1.y1 * mat2.y2) + (mat1.z1 * mat2.y3), (mat1.x1 * mat2.z1) + (mat1.y1 * mat2.z2) + (mat1.z1 * mat2.z3),
			(mat1.x2 * mat2.x1) + (mat1.y2 * mat2.x2) + (mat1.z2 * mat2.x3), (mat1.x2 * mat2.y1) + (mat1.y2 * mat2.y2) + (mat1.z2 * mat2.y3), (mat1.x2 * mat2.z1) + (mat1.y2 * mat2.z2) + (mat1.z2 * mat2.z3),
			(mat1.x3 * mat2.x1) + (mat1.y3 * mat2.x2) + (mat1.z3 * mat2.x3), (mat1.x3 * mat2.y1) + (mat1.y3 * mat2.y2) + (mat1.z3 * mat2.y3), (mat1.x3 * mat2.z1) + (mat1.y3 * mat2.z2) + (mat1.z3 * mat2.z3)
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