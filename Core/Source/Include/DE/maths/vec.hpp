#ifndef __DEEP_ENGINE_VEC_HPP__
#define __DEEP_ENGINE_VEC_HPP__

#include "DE/core/def.hpp"

#include <stdint.h>

namespace deep
{

    struct fmat2x2;
    struct fmat3x3;
    struct fmat4x4;

    template<typename Type>
    struct vec2
    {
        Type x;
        Type y;

        vec2(const Type &x = Type(), const Type &y = Type());

        vec2 operator-(const Type &value) const;
        vec2 operator-(const vec2 &vec) const;
        vec2 &operator-=(const Type &value) const;
        vec2 &operator-=(const vec2 &vec);
        vec2 operator+(const Type &value) const;
        vec2 operator+(const vec2 &vec) const;
        vec2 &operator+=(const Type &value);
        vec2 &operator+=(const vec2 &vec);
        vec2 operator*(const Type &value) const;
        vec2 &operator*=(const Type &value);
        vec2 operator/(const Type &value) const;
        vec2 &operator/=(const Type &value);

        vec2 xy() const;
        vec2 yx() const;

        static vec2 sub(const vec2 &vec, const Type &value);
        static vec2 sub(const vec2 &vec1, const vec2 &vec2);
        static vec2 add(const vec2 &vec, const Type &value);
        static vec2 add(const vec2 &vec1, const vec2 &vec2);
        static vec2 mul(const vec2 &vec, const Type &value);
        static vec2 mul(const vec2 &vec, const fmat2x2 &mat);
        static vec2 div(const vec2 &vec, const Type &value);

        static vec2 scale(const vec2 &v, const Type &scalar);
        static float magn(const vec2 &vec);
        static vec2 normalize(const vec2 &vec);
        // Le cross product n'est pas possible en 2D.
        static float dot(const vec2 &vec1, const vec2 &vec2);

        static vec2 inv(const vec2 &vec);
    };

    template<typename Type>
    struct vec3
    {
        Type x;
        Type y;
        Type z;

        vec3(const Type &x = Type(), const Type &y = Type(), const Type &z = Type());
        vec3(const vec2<Type> &vec, const Type &z = Type());

        vec3 operator-(const Type &value) const;
        vec3 operator-(const vec3 &vec) const;
        vec3 &operator-=(const Type &value);
        vec3 &operator-=(const vec3 &vec);
        vec3 operator+(const Type &value) const;
        vec3 operator+(const vec3 &vec) const;
        vec3 &operator+=(const Type &value);
        vec3 &operator+=(const vec3 &vec);
        vec3 operator*(const Type &value) const;
        vec3 &operator*=(const Type &value);
        vec3 operator/(const Type &value) const;
        vec3 &operator/=(const Type &value);

        vec3 xyz() const;
        vec3 xzy() const;
        vec3 yxz() const;
        vec3 yzx() const;
        vec3 zxy() const;
        vec3 zyx() const;

        vec2<Type> xy() const;
        vec2<Type> xz() const;
        vec2<Type> yx() const;
        vec2<Type> yz() const;
        vec2<Type> zx() const;
        vec2<Type> zy() const;

        static vec3 sub(const vec3 &vec, const Type &value);
        static vec3 sub(const vec3 &vec1, const vec3 &vec2);
        static vec3 add(const vec3 &vec, const Type &value);
        static vec3 add(const vec3 &vec1, const vec3 &vec2);
        static vec3 mul(const vec3 &vec, const Type &value);
        static vec3 mul(const vec3 &vec, const fmat3x3 &mat);
        static vec3 div(const vec3 &vec, const Type &value);

        static vec3 scale(const vec3 &vec, const Type &scalar);
        static float magn(const vec3 &vec);
        static vec3 normalize(const vec3 &vec);
        static vec3 cross(const vec3 &vec1, const vec3 &vec2);
        static float dot(const vec3 &vec1, const vec3 &vec2);

        static vec3 inv(const vec3 &vec);
    };

    template<typename Type>
    struct vec4
    {
        Type x;
        Type y;
        Type z;
        Type w;

        vec4(const Type &x = Type(), const Type &y = Type(), const Type &z = Type(), const Type &w = Type());
        vec4(const vec3<Type> &vec, const Type &w = Type());
        vec4(const Type &x, const vec3<Type> &vec);
        vec4(const vec2<Type> &vec, const Type &z = Type(), const Type &w = Type());
        vec4(const vec2<Type> &vec1, const vec2<Type> &vec2);
        vec4(const Type &x, const vec2<Type> &vec, const Type &w);

        vec3<Type> xyz() const;
        vec3<Type> rgb() const;
        vec3<Type> bgr() const;
    };

}

#include "DE/maths/vec_impl.hpp"

#endif