#ifndef __DEEP_ENGINE_MAT_HPP__
#define __DEEP_ENGINE_MAT_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/common.hpp"

#include <stdint.h>
#include <math.h>

namespace deep
{

    template<typename Type> struct vec2;
    template<typename Type> struct vec3;
    template<typename Type> struct vec4;

    template<typename Type>
    struct mat2x2
    {
        enum class index : uint8_t
        {
            x1 = 0, y1 = 1,
            x2 = 2, y2 = 3
        };

        Type data[2 * 2];

        mat2x2(const Type &x1 = 1, const Type &y1 = 0, const Type &x2 = 0, const Type &y2 = 1);

        mat2x2 &operator=(const mat2x2 &other);

        Type operator[](size_t index) const;
        Type &operator[](size_t index);

        vec2<Type> operator*(const vec2<Type> &vec) const;
        mat2x2 operator*(const Type &value) const;

        bool operator==(const mat2x2 &other) const;

        static mat2x2 mul(const mat2x2 &mat, const Type &value);
        static vec2<Type> mul(const mat2x2 &mat, const vec2<Type> &vec);

        static vec2<Type> scale(const vec2<Type> &vec, const Type &scale1, const Type &scale2);
        static vec2<Type> translate(const vec2<Type> &vec, const Type &x, const Type &y);
        static vec2<Type> rotate(const vec2<Type> &vec, const Type &degrees);

        static Type determinant(const mat2x2 &mat);

        static bool inverse(const mat2x2 &mat, mat2x2 &dest);
        static mat2x2 transpose(const mat2x2 &mat);
    };

    template<typename Type>
    struct mat3x3
    {
        enum class index : uint8_t
        {
            x1 = 0, y1 = 1, z1 = 2,
            x2 = 3, y2 = 4, z2 = 5,
            x3 = 6, y3 = 7, z3 = 8
        };

        Type data[3 * 3];

        mat3x3(
            const Type &x1 = 1, const Type &y1 = 0, const Type &z1 = 0,
            const Type &x2 = 0, const Type &y2 = 1, const Type &z2 = 0,
            const Type &x3 = 0, const Type &y3 = 0, const Type &z3 = 1
        );

        mat3x3 &operator=(const mat3x3 &other);

        mat3x3 operator*(const Type &value) const;
        vec3<Type> operator*(const vec3<Type> &vec) const;
        mat3x3 operator*(const mat3x3 &mat) const;

        Type operator[](size_t index) const;

        bool operator==(const mat3x3 &other) const;

        Type *ptr() const;

        static mat3x3 mul(const mat3x3 &mat, const Type &value);
        static vec3<Type> mul(const mat3x3 &mat, const vec3<Type> &vec);
        static mat3x3 mul(const mat3x3 &mat1, const mat3x3 &mat2);

        static mat3x3 add(const mat3x3 &mat1, const mat3x3 &mat2);

        static mat3x3 translate(const mat3x3 &mat, const vec2<Type> &vec);
        static mat3x3 scale(const mat3x3 &mat, const vec2<Type> &vec);
        static mat3x3 scale(const mat3x3 &mat, const Type &value);
        static mat3x3 rotate(const mat3x3 &mat, const Type &degrees);

        static Type determinant(const mat3x3 &mat);

        static bool inverse(const mat3x3 &mat, mat3x3 &dest);
        static mat3x3 transpose(const mat3x3 &mat);

    };

    template<typename Type>
    struct mat4x4_row
    {
        enum class index : uint8_t
        {
            x = 0,
            y = 1,
            z = 2,
            w = 3
        };

        Type data[4];

        mat4x4_row(const Type &x = 0, const Type &y = 0, const Type &z = 0, const Type &w = 0);
        mat4x4_row(const mat4x4_row &other);

        mat4x4_row &operator=(const mat4x4_row &other);

        Type &operator[](size_t index) const;

        mat4x4_row &operator*=(const Type &value);
        mat4x4_row &operator*=(const mat4x4_row &other);
        mat4x4_row &operator/=(const Type &value);
        mat4x4_row &operator/=(const mat4x4_row &other);
        mat4x4_row &operator+=(const Type &value);
        mat4x4_row &operator+=(const mat4x4_row &other);
        mat4x4_row &operator-=(const Type &value);
        mat4x4_row &operator-=(const mat4x4_row &other);

    };

    template<typename Type>
    struct mat4x4
    {
        enum class index : uint8_t
        {
            x1 = 0,  y1 = 1,  z1 = 2,  w1 = 3,
            x2 = 4,  y2 = 5,  z2 = 6,  w2 = 7,
            x3 = 8,  y3 = 9,  z3 = 10, w3 = 11,
            x4 = 12, y4 = 13, z4 = 14, w4 = 15
        };

        Type data[4 * 4];

        mat4x4(
            const Type &x1 = 1, const Type &y1 = 0, const Type &z1 = 0, const Type &w1 = 0,
            const Type &x2 = 0, const Type &y2 = 1, const Type &z2 = 0, const Type &w2 = 0,
            const Type &x3 = 0, const Type &y3 = 0, const Type &z3 = 1, const Type &w3 = 0,
            const Type &x4 = 0, const Type &y4 = 0, const Type &z4 = 0, const Type &w4 = 1
        );

        mat4x4(const mat4x4_row<Type> &row);
        mat4x4(
            const mat4x4_row<Type> &row1,
            const mat4x4_row<Type> &row2,
            const mat4x4_row<Type> &row3,
            const mat4x4_row<Type> &row4
        );

        mat4x4 operator*(const mat4x4 &mat) const;

        Type operator[](size_t index) const;
        Type &operator[](size_t index);

        bool operator==(const mat4x4 &other) const;

        static void make_pivot_row(uint8_t pivotIndex, mat4x4_row<Type> &leftRow, mat4x4_row<Type> &rightRow);
        static void calc_inverse_row(uint8_t pivotIndex, mat4x4_row<Type> fromLeftRow, mat4x4_row<Type> fromRightRow, mat4x4_row<Type> &toLeftRow, mat4x4_row<Type> &toRightRow);

        mat4x4_row<Type> get_row_1() const;
        mat4x4_row<Type> get_row_2() const;
        mat4x4_row<Type> get_row_3() const;
        mat4x4_row<Type> get_row_4() const;

        static mat4x4 mul(const mat4x4 &mat1, const mat4x4 &mat2);

        Type *ptr() const;

        static mat4x4 translate(const mat4x4 &mat, const vec3<Type> &vec);
        static mat4x4 scale(const mat4x4 &mat, const vec3<Type> &vec);
        static mat4x4 rotate_x(const mat4x4 &mat, const Type &degrees);
        static mat4x4 rotate_y(const mat4x4 &mat, const Type &degrees);
        static mat4x4 rotate_z(const mat4x4 &mat, const Type &degrees);
        static mat4x4 perspective(const mat4x4 &mat, const Type &fovy, const Type &aspectRatio, const Type &znear, const Type &zfar);

        static Type determinant(const mat4x4 &mat);

        static bool inverse(const mat4x4 &mat, mat4x4 &dest);
        static mat4x4 transpose(const mat4x4 &mat);

    };

}

#include "maths/mat_impl.hpp"

#endif