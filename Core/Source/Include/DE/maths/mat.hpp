#ifndef __DEEP_ENGINE_MAT_HPP__
#define __DEEP_ENGINE_MAT_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/common.hpp"

#include <stdint.h>
#include <math.h>

namespace deep
{

	template<typename Type> struct vec2;
	template<typename Type> struct vec3;
	template<typename Type> struct vec4;

	struct fmat2x2
    {
		float x1;
		float x2;
		float y1;
		float y2;

		DE_API fmat2x2(float x1 = 1.0f, float y1 = 0.0f, float x2 = 0.0f, float y2 = 1.0f);

		DE_API vec2<float> operator*(const vec2<float> &vec) const;

		DE_API static vec2<float> mul(const fmat2x2 &mat, const vec2<float> &vec);

		DE_API static vec2<float> scale(const vec2<float> &vec, float scale1, float scale2);
		DE_API static vec2<float> translate(const vec2<float> &vec, float x, float y);
		DE_API static vec2<float> rotate(const vec2<float> &vec, float degrees);
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

		DE_API vec3<float> operator*(const vec3<float> &vec) const;
		DE_API fmat3x3 operator*(const fmat3x3 &mat) const;
		DE_API float operator[](size_t index) const;


		DE_API float *ptr();

		DE_API static vec3<float> mul(const fmat3x3 &mat, const vec3<float> &vec);
		DE_API static fmat3x3 mul(const fmat3x3 &mat1, const fmat3x3 &mat2);

		DE_API static fmat3x3 translate(const fmat3x3 &mat, const vec2<float> &vec);
		DE_API static fmat3x3 scale(const fmat3x3 &mat, const vec2<float> &vec);
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

		DE_API static fmat4x4 translate(const fmat4x4 &mat, const vec3<float> &vec);
		DE_API static fmat4x4 scale(const fmat4x4 &mat, const vec3<float> &vec);
		DE_API static fmat4x4 rotate_x(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 rotate_y(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 rotate_z(const fmat4x4 &mat, float degrees);
		DE_API static fmat4x4 perspective(const fmat4x4 &mat, float fovy, float aspectRatio, float znear, float zfar);

	};

}

#include "DE/maths/mat_impl.hpp"

#endif