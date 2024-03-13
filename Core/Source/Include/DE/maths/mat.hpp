#ifndef __DEEP_ENGINE_MAT_HPP__
#define __DEEP_ENGINE_MAT_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/common.hpp"
#include "DE/maths/vec.hpp"

#include <stdint.h>
#include <math.h>

namespace deep
{

	struct fmat2x2
    {
		float x1;
		float x2;
		float y1;
		float y2;

		DE_API fmat2x2(float x1 = 1.0f, float y1 = 0.0f, float x2 = 0.0f, float y2 = 1.0f);

		DE_API fvec2 operator*(const fvec2 &vec) const;

		DE_API static fvec2 mul(const fmat2x2 &mat, const fvec2 &vec);

		DE_API static fvec2 scale(const fvec2 &vec, float scale1, float scale2);
		DE_API static fvec2 translate(const fvec2 &vec, float x, float y);
		DE_API static fvec2 rotate(const fvec2 &vec, float degrees);
	};

	enum class fmat3x3_index : uint8_t
    {
		x1 = 0, y1 = 1, z1 = 2,
		x2 = 3, y2 = 4, z2 = 5,
		x3 = 6, y3 = 7, z3 = 8
	};

	struct fmat3x3
    {

		float data[3 * 3];

		DE_API fmat3x3(
			float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f,
			float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f,
			float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f
		);

		DE_API fvec3 operator*(const fvec3 &vec) const;
		DE_API fmat3x3 operator*(const fmat3x3 &mat) const;
		DE_API float operator[](size_t index) const;


		DE_API float *ptr();

		DE_API static fvec3 mul(const fmat3x3 &mat, const fvec3 &vec);
		DE_API static fmat3x3 mul(const fmat3x3 &mat1, const fmat3x3 &mat2);

		DE_API static fmat3x3 translate(const fmat3x3 &mat, const fvec2 &vec);
		DE_API static fmat3x3 scale(const fmat3x3 &mat, const fvec2 &vec);
		DE_API static fmat3x3 rotate(const fmat3x3 &mat, float degrees);

	};

	enum class fmat4x4_index : uint8_t
    {
		x1 = 0,  y1 = 1,  z1 = 2,  w1 = 3,
		x2 = 4,  y2 = 5,  z2 = 6,  w2 = 7,
		x3 = 8,  y3 = 9,  z3 = 10, w3 = 11,
		x4 = 12, y4 = 13, z4 = 14, w4 = 15
	};

	struct fmat4x4
    {

		float data[4 * 4];

		DE_API fmat4x4(
			float x1 = 1.0f, float y1 = 0.0f, float z1 = 0.0f, float w1 = 0.0f,
			float x2 = 0.0f, float y2 = 1.0f, float z2 = 0.0f, float w2 = 0.0f,
			float x3 = 0.0f, float y3 = 0.0f, float z3 = 1.0f, float w3 = 0.0f,
			float x4 = 0.0f, float y4 = 0.0f, float z4 = 0.0f, float w4 = 1.0f
		);

		DE_API static fmat4x4 mul(const fmat4x4 &mat1, const fmat4x4 &mat2);

		DE_API fmat4x4 operator*(const fmat4x4 &mat) const;
		DE_API float operator[](size_t index) const;
		DE_API float &operator[](size_t index);

		DE_API float *ptr();

		DE_API static fmat4x4 translate(const fmat4x4 &mat, const fvec3 &vec);
		DE_API static fmat4x4 scale(const fmat4x4 &mat, const fvec3 &vec);
		DE_API static fmat4x4 rotate_x(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 rotate_y(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 rotate_z(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 perspective(const fmat4x4 &mat, float fovy, float aspectRatio, float znear, float zfar);

	};
	/*
	============
	fmat4x4::mul
	============
	*/
	inline fmat4x4 fmat4x4::mul(const fmat4x4 &mat1, const fmat4x4 &mat2)
	{

		return
        {
			(
                mat1[to_utype(fmat4x4_index::x1)] * 
                mat2[to_utype(fmat4x4_index::x1)] + 
                mat1[to_utype(fmat4x4_index::y1)] * 
                mat2[to_utype(fmat4x4_index::x2)] + 
                mat1[to_utype(fmat4x4_index::z1)] * 
                mat2[to_utype(fmat4x4_index::x3)] + 
                mat1[to_utype(fmat4x4_index::w1)] * 
                mat2[to_utype(fmat4x4_index::x4)]
            ),
            (
                mat1[to_utype(fmat4x4_index::x1)] * 
                mat2[to_utype(fmat4x4_index::y1)] + 
                mat1[to_utype(fmat4x4_index::y1)] * 
                mat2[to_utype(fmat4x4_index::y2)] + 
                mat1[to_utype(fmat4x4_index::z1)] * 
                mat2[to_utype(fmat4x4_index::y3)] + 
                mat1[to_utype(fmat4x4_index::w1)] * 
                mat2[to_utype(fmat4x4_index::y4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x1)] * 
                mat2[to_utype(fmat4x4_index::z1)] + 
                mat1[to_utype(fmat4x4_index::y1)] * 
                mat2[to_utype(fmat4x4_index::z2)] + 
                mat1[to_utype(fmat4x4_index::z1)] * 
                mat2[to_utype(fmat4x4_index::z3)] + 
                mat1[to_utype(fmat4x4_index::w1)] * 
                mat2[to_utype(fmat4x4_index::z4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x1)] * 
                mat2[to_utype(fmat4x4_index::w1)] + 
                mat1[to_utype(fmat4x4_index::y1)] * 
                mat2[to_utype(fmat4x4_index::w2)] + 
                mat1[to_utype(fmat4x4_index::z1)] * 
                mat2[to_utype(fmat4x4_index::w3)] + 
                mat1[to_utype(fmat4x4_index::w1)] * 
                mat2[to_utype(fmat4x4_index::w4)]
            ),

            (
                mat1[to_utype(fmat4x4_index::x2)] * 
                mat2[to_utype(fmat4x4_index::x1)] + 
                mat1[to_utype(fmat4x4_index::y2)] * 
                mat2[to_utype(fmat4x4_index::x2)] + 
                mat1[to_utype(fmat4x4_index::z2)] * 
                mat2[to_utype(fmat4x4_index::x3)] + 
                mat1[to_utype(fmat4x4_index::w2)] * 
                mat2[to_utype(fmat4x4_index::x4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x2)] * 
                mat2[to_utype(fmat4x4_index::y1)] + 
                mat1[to_utype(fmat4x4_index::y2)] * 
                mat2[to_utype(fmat4x4_index::y2)] + 
                mat1[to_utype(fmat4x4_index::z2)] * 
                mat2[to_utype(fmat4x4_index::y3)] + 
                mat1[to_utype(fmat4x4_index::w2)] * 
                mat2[to_utype(fmat4x4_index::y4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x2)] * 
                mat2[to_utype(fmat4x4_index::z1)] + 
                mat1[to_utype(fmat4x4_index::y2)] * 
                mat2[to_utype(fmat4x4_index::z2)] + 
                mat1[to_utype(fmat4x4_index::z2)] * 
                mat2[to_utype(fmat4x4_index::z3)] + 
                mat1[to_utype(fmat4x4_index::w2)] * 
                mat2[to_utype(fmat4x4_index::z4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x2)] * 
                mat2[to_utype(fmat4x4_index::w1)] + 
                mat1[to_utype(fmat4x4_index::y2)] * 
                mat2[to_utype(fmat4x4_index::w2)] + 
                mat1[to_utype(fmat4x4_index::z2)] * 
                mat2[to_utype(fmat4x4_index::w3)] + 
                mat1[to_utype(fmat4x4_index::w2)] * 
                mat2[to_utype(fmat4x4_index::w4)]
            ),
			(
                mat1[to_utype(fmat4x4_index::x3)] * 
                mat2[to_utype(fmat4x4_index::x1)] + 
                mat1[to_utype(fmat4x4_index::y3)] * 
                mat2[to_utype(fmat4x4_index::x2)] + 
                mat1[to_utype(fmat4x4_index::z3)] * 
                mat2[to_utype(fmat4x4_index::x3)] + 
                mat1[to_utype(fmat4x4_index::w3)] * 
                mat2[to_utype(fmat4x4_index::x4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x3)] * 
                mat2[to_utype(fmat4x4_index::y1)] + 
                mat1[to_utype(fmat4x4_index::y3)] * 
                mat2[to_utype(fmat4x4_index::y2)] + 
                mat1[to_utype(fmat4x4_index::z3)] * 
                mat2[to_utype(fmat4x4_index::y3)] + 
                mat1[to_utype(fmat4x4_index::w3)] * 
                mat2[to_utype(fmat4x4_index::y4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x3)] * 
                mat2[to_utype(fmat4x4_index::z1)] + 
                mat1[to_utype(fmat4x4_index::y3)] * 
                mat2[to_utype(fmat4x4_index::z2)] + 
                mat1[to_utype(fmat4x4_index::z3)] * 
                mat2[to_utype(fmat4x4_index::z3)] + 
                mat1[to_utype(fmat4x4_index::w3)] * 
                mat2[to_utype(fmat4x4_index::z4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x3)] * 
                mat2[to_utype(fmat4x4_index::w1)] + 
                mat1[to_utype(fmat4x4_index::y3)] * 
                mat2[to_utype(fmat4x4_index::w2)] + 
                mat1[to_utype(fmat4x4_index::z3)] * 
                mat2[to_utype(fmat4x4_index::w3)] + 
                mat1[to_utype(fmat4x4_index::w3)] * 
                mat2[to_utype(fmat4x4_index::w4)]
            ),
			(
                mat1[to_utype(fmat4x4_index::x4)] * 
                mat2[to_utype(fmat4x4_index::x1)] + 
                mat1[to_utype(fmat4x4_index::y4)] * 
                mat2[to_utype(fmat4x4_index::x2)] + 
                mat1[to_utype(fmat4x4_index::z4)] * 
                mat2[to_utype(fmat4x4_index::x3)] + 
                mat1[to_utype(fmat4x4_index::w4)] * 
                mat2[to_utype(fmat4x4_index::x4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x4)] * 
                mat2[to_utype(fmat4x4_index::y1)] + 
                mat1[to_utype(fmat4x4_index::y4)] * 
                mat2[to_utype(fmat4x4_index::y2)] + 
                mat1[to_utype(fmat4x4_index::z4)] * 
                mat2[to_utype(fmat4x4_index::y3)] + 
                mat1[to_utype(fmat4x4_index::w4)] * 
                mat2[to_utype(fmat4x4_index::y4)]
            ),
            (
                mat1[to_utype(fmat4x4_index::x4)] * 
                mat2[to_utype(fmat4x4_index::z1)] + 
                mat1[to_utype(fmat4x4_index::y4)] * 
                mat2[to_utype(fmat4x4_index::z2)] + 
                mat1[to_utype(fmat4x4_index::z4)] * 
                mat2[to_utype(fmat4x4_index::z3)] + 
                mat1[to_utype(fmat4x4_index::w4)] * 
                mat2[to_utype(fmat4x4_index::z4)]
            ), 
            (
                mat1[to_utype(fmat4x4_index::x4)] *
                mat2[to_utype(fmat4x4_index::w1)] + 
                mat1[to_utype(fmat4x4_index::y4)] * 
                mat2[to_utype(fmat4x4_index::w2)] + 
                mat1[to_utype(fmat4x4_index::z4)] * 
                mat2[to_utype(fmat4x4_index::w3)] + 
                mat1[to_utype(fmat4x4_index::w4)] * 
                mat2[to_utype(fmat4x4_index::w4)]
            )
		};
	}

	/*
	==================
	fmat4x4::operator*
	==================
	*/
	inline fmat4x4 fmat4x4::operator*(const fmat4x4 &mat) const
	{
		return mul(*this, mat);
	}

	/*
	===================
	fmat4x4::operator[]
	===================
	*/
	inline float fmat4x4::operator[](size_t index) const
	{
		return data[index];
	}

	inline float &fmat4x4::operator[](size_t index)
	{
		return data[index];
	}

	/*
	============
	fmat4x4::ptr
	============
	*/
	inline float *fmat4x4::ptr()
	{
		return data;
	}

	/*
	==================
	fmat4x4::translate
	==================
	*/
	inline fmat4x4 fmat4x4::translate(const fmat4x4 &mat, const fvec3 &vec)
	{
		return mul(
			mat,
			{
				1.0f, 0.0f, 0.0f, vec.x,
				0.0f, 1.0f, 0.0f, vec.y,
				0.0f, 0.0f, 1.0f, vec.z,
				0.0f, 0.0f, 0.0f, 1.0f
			}
		);
	}

	/*
	==============
	fmat4x4::scale
	==============
	*/
	inline fmat4x4 fmat4x4::scale(const fmat4x4 &mat, const fvec3 &vec)
	{
		return mul(
			mat,
			{
				vec.x, 0.0f,  0.0f,  0.0f,
				0.0f,  vec.y, 0.0f,  0.0f,
				0.0f,  0.0f,  vec.z, 0.0f,
				0.0f,  0.0f,  0.0f,  1.0f
			}
		);
	}

	/*
	=================
	fmat4x4::rotate_x
	=================
	*/
	inline fmat4x4 fmat4x4::rotate_x(const fmat4x4 &mat, float degrees)
	{
		float rad = deg_to_rad(degrees);
		return mul(
			mat,
			{
				1.0f,  0.0f,       0.0f,      0.0f,
				0.0f,  cosf(rad), -sinf(rad), 0.0f,
				0.0f,  sinf(rad),  cosf(rad), 0.0f,
				0.0f,  0.0f,       0.0f,      1.0f
			}
		);
	}

	/*
	=================
	fmat4x4::rotate_y
	=================
	*/
	inline fmat4x4 fmat4x4::rotate_y(const fmat4x4 &mat, float degrees)
	{
		float rad = deg_to_rad(degrees);
		return mul(
			mat,
			{
				 cosf(rad), 0.0f, sinf(rad), 0.0f,
				 0.0f,      1.0f, 0.0f,      0.0f,
				-sinf(rad), 0.0f, cosf(rad), 0.0f,
				 0.0f,      0.0f, 0.0f,      1.0f
			}
		);
	}

	/*
	=================
	fmat4x4::rotate_z
	=================
	*/
	inline fmat4x4 fmat4x4::rotate_z(const fmat4x4 &mat, float degrees)
	{
		float rad = deg_to_rad(degrees);
		return mul(
			mat,
			{
				cosf(rad), -sinf(rad), 0.0f, 0.0f,
				sinf(rad),  cosf(rad), 0.0f, 0.0f,
				0.0f,       0.0f,      1.0f, 0.0f,
				0.0f,       0.0f,      0.0f, 1.0f
			}
		);
	}

	/*
	====================
	fmat4x4::perspective
	====================
	*/
	inline fmat4x4 fmat4x4::perspective(const fmat4x4 &mat, float fovy, float aspectRatio, float znear, float zfar)
	{
		float tanHalfFovy = tanf(fovy / 2.0f);
		float zr = zfar - znear;

		return
        {
			mat[to_utype(fmat4x4_index::x1)] / (aspectRatio * tanHalfFovy), 0.0f,                                   0.0f,                 0.0f,
			0.0f,                                                 mat[to_utype(fmat4x4_index::y2)] / tanHalfFovy,   0.0f,                 0.0f,
			0.0f,                                                 0.0f,                                 -(zfar + znear) / zr, -(2.0f * zfar * znear) / zr,
			0.0f,                                                 0.0f,                                   -1.0f,                0.0f
		};
	}

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
		return
        {
			(mat1[to_utype(fmat3x3_index::x1)] * mat2[to_utype(fmat3x3_index::x1)]) + 
            (mat1[to_utype(fmat3x3_index::y1)] * mat2[to_utype(fmat3x3_index::x2)]) + 
            (mat1[to_utype(fmat3x3_index::z1)] * mat2[to_utype(fmat3x3_index::x3)]), 
            (mat1[to_utype(fmat3x3_index::x1)] * mat2[to_utype(fmat3x3_index::y1)]) + 
            (mat1[to_utype(fmat3x3_index::y1)] * mat2[to_utype(fmat3x3_index::y2)]) + 
            (mat1[to_utype(fmat3x3_index::z1)] * mat2[to_utype(fmat3x3_index::y3)]), 
            (mat1[to_utype(fmat3x3_index::x1)] * mat2[to_utype(fmat3x3_index::z1)]) + 
            (mat1[to_utype(fmat3x3_index::y1)] * mat2[to_utype(fmat3x3_index::z2)]) + 
            (mat1[to_utype(fmat3x3_index::z1)] * mat2[to_utype(fmat3x3_index::z3)]),
			(mat1[to_utype(fmat3x3_index::x2)] * mat2[to_utype(fmat3x3_index::x1)]) + 
            (mat1[to_utype(fmat3x3_index::y2)] * mat2[to_utype(fmat3x3_index::x2)]) + 
            (mat1[to_utype(fmat3x3_index::z2)] * mat2[to_utype(fmat3x3_index::x3)]), 
            (mat1[to_utype(fmat3x3_index::x2)] * mat2[to_utype(fmat3x3_index::y1)]) + 
            (mat1[to_utype(fmat3x3_index::y2)] * mat2[to_utype(fmat3x3_index::y2)]) + 
            (mat1[to_utype(fmat3x3_index::z2)] * mat2[to_utype(fmat3x3_index::y3)]), 
            (mat1[to_utype(fmat3x3_index::x2)] * mat2[to_utype(fmat3x3_index::z1)]) + 
            (mat1[to_utype(fmat3x3_index::y2)] * mat2[to_utype(fmat3x3_index::z2)]) + 
            (mat1[to_utype(fmat3x3_index::z2)] * mat2[to_utype(fmat3x3_index::z3)]),
			(mat1[to_utype(fmat3x3_index::x3)] * mat2[to_utype(fmat3x3_index::x1)]) + 
            (mat1[to_utype(fmat3x3_index::y3)] * mat2[to_utype(fmat3x3_index::x2)]) + 
            (mat1[to_utype(fmat3x3_index::z3)] * mat2[to_utype(fmat3x3_index::x3)]), 
            (mat1[to_utype(fmat3x3_index::x3)] * mat2[to_utype(fmat3x3_index::y1)]) + 
            (mat1[to_utype(fmat3x3_index::y3)] * mat2[to_utype(fmat3x3_index::y2)]) + 
            (mat1[to_utype(fmat3x3_index::z3)] * mat2[to_utype(fmat3x3_index::y3)]), 
            (mat1[to_utype(fmat3x3_index::x3)] * mat2[to_utype(fmat3x3_index::z1)]) + 
            (mat1[to_utype(fmat3x3_index::y3)] * mat2[to_utype(fmat3x3_index::z2)]) + 
            (mat1[to_utype(fmat3x3_index::z3)] * mat2[to_utype(fmat3x3_index::z3)])
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
		float rad = deg_to_rad(degrees);
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
		float rad = deg_to_rad(degrees);
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