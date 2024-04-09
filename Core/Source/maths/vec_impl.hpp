#ifndef __DEEP_ENGINE_VEC_IMPL_HPP__
#define __DEEP_ENGINE_VEC_IMPL_HPP__

#include "maths/vec.hpp"
#include "maths/mat.hpp"

#include <math.h>

namespace deep
{

    template<typename Type>
    vec2<Type>::vec2(const Type &_x, const Type &_y)
        : x(_x),
          y(_y)
    { }

    template<typename Type>
    vec2<Type> vec2<Type>::operator-(const Type &value) const
    {
        return sub(*this, value);
    }

    template<typename Type>
    vec2<Type> vec2<Type>::operator-(const vec2 &vec) const
    {
        return sub(*this, vec);
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator-=(const Type &value) const
    {
        *this = sub(*this, value);

        return *this;
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator-=(const vec2 &vec)
    {
        *this = sub(*this, vec);

        return *this;
    }

    template<typename Type>
    vec2<Type> vec2<Type>::operator+(const Type &value) const
    {
        return add(*this, value);
    }

    template<typename Type>
    vec2<Type> vec2<Type>::operator+(const vec2 &vec) const
    {
        return add(*this, vec);
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator+=(const Type &value)
    {
        *this = add(*this, value);

        return *this;
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator+=(const vec2 &vec)
    {
        *this = add(*this, vec);

        return *this;
    }

    template<typename Type>
    vec2<Type> vec2<Type>::operator*(const Type &value) const
    {
        return mul(*this, value);
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator*=(const Type &value)
    {
        *this = mul(*this, value);

        return *this;
    }

    template<typename Type>
    vec2<Type> vec2<Type>::operator/(const Type &value) const
    {
        return div(*this, value);
    }

    template<typename Type>
    vec2<Type> &vec2<Type>::operator/=(const Type &value)
    {
        *this = div(*this, value);

        return *this;
    }

    template<typename Type>
    vec2<Type> vec2<Type>::xy() const
    {
        return { x, y };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::yx() const
    {
        return { y, x };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::sub(const vec2 &vec, const Type &value)
    {
        return
        {
            vec.x - value,
            vec.y - value
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::sub(const vec2 &vec1, const vec2 &vec2)
    {
        return
        {
            vec1.x - vec2.x,
            vec1.y - vec2.y
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::add(const vec2 &vec, const Type &value)
    {
        return
        {
            vec.x + value,
            vec.y + value
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::add(const vec2 &vec1, const vec2 &vec2)
    {
        return
        {
            vec1.x + vec2.x,
            vec1.y + vec2.y
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::mul(const vec2 &vec, const Type &value)
    {
        return
        {
            vec.x * value,
            vec.y * value
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::mul(const vec2 &vec, const mat2x2<Type> &mat)
    {
        return
        {
            vec.x * mat[0] + vec.y * mat[2],
            vec.x * mat[1] + vec.y * mat[3]
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::div(const vec2 &vec, const Type &value)
    {
        return
        {
            vec.x / value,
            vec.y / value
        };
    }

    template<typename Type>
    vec2<Type> vec2<Type>::scale(const vec2 &v, const Type &scalar)
    {
        return
        {
            vec.x * scalar,
            vec.y * scalar
        };
    }

    template<typename Type>
    float vec2<Type>::magn(const vec2 &vec)
    {
        return sqrtf(vec.x * vec.x + vec.y * vec.y);
    }

    template<typename Type>
    vec2<Type> vec2<Type>::normalize(const vec2 &vec)
    {
        float length = magn(vec);

        return
        {
            vec.x / length,
            vec.y / length
        };
    }

    template<typename Type>
    float vec2<Type>::dot(const vec2 &vec1, const vec2 &vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    template<typename Type>
    vec2<Type> vec2<Type>::inv(const vec2 &vec)
    {
        return scale(*this, static_cast<Type>(-1));
    }

    template<typename Type>
    vec3<Type>::vec3(const Type &_x, const Type &_y, const Type &_z)
        : x(_x),
          y(_y),
          z(_z)
    { }

    template<typename Type>
    vec3<Type>::vec3(const vec2<Type> &vec, const Type &_z)
        : x(vec.x),
          y(vec.y),
          z(_z)
    { }

    template<typename Type>
    vec3<Type> vec3<Type>::mul(const vec3 &vec, const mat3x3<Type> &mat)
    {
        return {
            (vec.x * mat[static_cast<uint8_t>(index::x1)]) + (vec.y * mat[static_cast<uint8_t>(index::y1)]) + (vec.z * mat[static_cast<uint8_t>(index::z1)]),
            (vec.x * mat[static_cast<uint8_t>(index::x2)]) + (vec.y * mat[static_cast<uint8_t>(index::y2)]) + (vec.z * mat[static_cast<uint8_t>(index::z2)]),
            (vec.x * mat[static_cast<uint8_t>(index::x3)]) + (vec.y * mat[static_cast<uint8_t>(index::y3)]) + (vec.z * mat[static_cast<uint8_t>(index::z3)])
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::div(const vec3 &vec, const Type &value)
    {
        return
        {
            vec.x / value,
            vec.y / value,
            vec.z / value
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator-(const Type &value) const
    {
        return sub(*this, value);
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator-(const vec3 &vec) const
    {
        return sub(*this, vec);
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator-=(const Type &value)
    {
        *this = sub(*this, value);

        return *this;
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator-=(const vec3 &vec)
    {
        *this = sub(*this, vec);

        return *this;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator+(const Type &value) const
    {
        return add(*this, value);
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator+(const vec3 &vec) const
    {
        return add(*this, vec);
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator+=(const Type &value)
    {
        *this = add(*this, value);

        return *this;
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator+=(const vec3 &vec)
    {
        *this = add(*this, vec);

        return *this;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator*(const Type &value) const
    {
        return mul(*this, value);
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator*=(const Type &value)
    {
        *this = mul(*this, value);

        return *this;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator/(const Type &value) const
    {
        return div(*this, value);
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator/=(const Type &value)
    {
        *this = div(*this, value);

        return *this;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::xyz() const
    {
        return { x, y, z };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::xzy() const
    {
        return { x, z, y };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::yxz() const
    {
        return { y, x, z };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::yzx() const
    {
        return { y, z, x };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::zxy() const
    {
        return { z, x, y };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::zyx() const
    {
        return { z, y, x };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::xy() const
    {
        return { x, y };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::xz() const
    {
        return { x, z };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::yx() const
    {
        return { y, x };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::yz() const
    {
        return { y, z };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::zx() const
    {
        return { z, x };
    }

    template<typename Type>
    vec2<Type> vec3<Type>::zy() const
    {
        return { z, y };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::sub(const vec3 &vec, const Type &value)
    {
        return
        {
            vec.x - value,
            vec.y - value,
            vec.z - value
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::sub(const vec3 &vec1, const vec3 &vec2)
    {
        return
        {
            vec1.x - vec2.x,
            vec1.y - vec2.y,
            vec1.z - vec2.z
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::add(const vec3 &vec, const Type &value)
    {
        return
        {
            vec.x + value,
            vec.y + value,
            vec.z + value
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::add(const vec3 &vec1, const vec3 &vec2)
    {
        return
        {
            vec1.x + vec2.x,
            vec1.y + vec2.y,
            vec1.z + vec2.z
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::mul(const vec3 &vec, const Type &value)
    {
        return
        {
            vec.x * value,
            vec.y * value,
            vec.z * value
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::scale(const vec3 &vec, const Type &scalar)
    {
        return
        {
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar
        };
    }

    template<typename Type>
    float vec3<Type>::magn(const vec3 &vec)
    {
        return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    template<typename Type>
    vec3<Type> vec3<Type>::normalize(const vec3 &vec)
    {
        float length = magn(vec);

        return
        {
            vec.x / length,
            vec.y / length,
            vec.z / length
        };
    }

    template<typename Type>
    vec3<Type> vec3<Type>::cross(const vec3 &vec1, const vec3 &vec2)
    {
        return
        {
            vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x
        };
    }

    template<typename Type>
    float vec3<Type>::dot(const vec3 &vec1, const vec3 &vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::inv(const vec3 &vec)
    {
        return scale(*this, static_cast<Type>(-1));
    }

    template<typename Type>
    vec4<Type>::vec4(const Type &_x, const Type &_y, const Type &_z, const Type &_w)
        : x(_x),
          y(_y),
          z(_z),
          w(_w)
    { }
    
    template<typename Type>
    vec4<Type>::vec4(const vec3<Type> &vec, const Type &_w)
        : x(vec.x),
          y(vec.y),
          z(vec.z),
          w(_w)
    { }

    template<typename Type>
    vec4<Type>::vec4(const Type &_x, const vec3<Type> &vec)
        : x(_x),
          y(vec.x),
          z(vec.y),
          w(vec.z)
    { }

    template<typename Type>
    vec4<Type>::vec4(const vec2<Type> &vec, const Type &_z, const Type &_w)
        : x(vec.x),
          y(vec.y),
          z(_z),
          w(_w)
    { }

    template<typename Type>
    vec4<Type>::vec4(const vec2<Type> &vec1, const vec2<Type> &vec2)
        : x(vec1.x),
          y(vec1.y),
          z(vec2.x),
          w(vec2.y)
    { }

    template<typename Type>
    vec4<Type>::vec4(const Type &_x, const vec2<Type> &vec, const Type &_w)
        : x(_x),
          y(vec.x),
          z(vec.y),
          w(_w)
    { }

    template<typename Type>
    vec3<Type> vec4<Type>::xyz() const
    {
        return
        {
            x,
            y,
            z
        }
    }
    
    template<typename Type>
    vec3<Type> vec4<Type>::rgb() const
    {
        return
        {
            x,
            y,
            z
        };
    }

    template<typename Type>
    vec3<Type> vec4<Type>::bgr() const
    {
        return
        {
            z,
            y,
            x
        };
    }

}

#endif