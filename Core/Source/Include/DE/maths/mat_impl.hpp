#ifndef __DEEP_ENGINE_MAT_IMPL_HPP__
#define __DEEP_ENGINE_MAT_IMPL_HPP__

#include "DE/maths/vec.hpp"
#include "DE/maths/mat.hpp"

namespace deep
{

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
	inline fmat4x4 fmat4x4::translate(const fmat4x4 &mat, const vec3<float> &vec)
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
	inline fmat4x4 fmat4x4::scale(const fmat4x4 &mat, const vec3<float> &vec)
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
	inline vec2<float> fmat2x2::operator*(const vec2<float> &vec) const
	{
		return mul(*this, vec);
	}

	/*
	==================
	fmat3x3::operator*
	==================
	*/
	inline vec3<float> fmat3x3::operator*(const vec3<float> &vec) const
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
	inline vec2<float> fmat2x2::mul(const fmat2x2 &mat, const vec2<float> &vec)
	{
		return vec2<float>::mul(vec, mat);
	}

	/*
	============
	fmat3x3::mul
	============
	*/
	inline vec3<float> fmat3x3::mul(const fmat3x3 &mat, const vec3<float> &vec)
	{
		return vec3<float>::mul(vec, mat);
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
	inline vec2<float> fmat2x2::scale(const vec2<float> &vec, float scale1, float scale2)
	{
		return vec2<float>::mul(vec, { scale1, 0.0f, 0.0f, scale2 } );
	}

	inline vec2<float> fmat2x2::translate(const vec2<float> &vec, float x, float y)
	{
		return { vec.x + x, vec.y + y };
	}

	/*
	===============
	fmat2x2::rotate
	===============
	*/
	inline vec2<float> fmat2x2::rotate(const vec2<float> &vec, float degrees)
	{
		float rad = deg_to_rad(degrees);

		return vec2<float>::mul(vec, {
			cosf(rad), -sinf(rad),
			sinf(rad), cosf(rad) 
		});
	}

	/*
	==================
	fmat3x3::translate
	==================
	*/
	inline fmat3x3 fmat3x3::translate(const fmat3x3 &mat, const vec2<float> &vec)
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
	inline fmat3x3 fmat3x3::scale(const fmat3x3 &mat, const vec2<float> &vec)
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