#ifndef __DEEP_ENGINE_VEC_HPP__
#define __DEEP_ENGINE_VEC_HPP__

#include "DE/core/def.hpp"

#include <stdint.h>

namespace deep
{

    class renderer;

    struct fmat2x2;
    struct fmat3x3;
    struct fmat4x4;

    enum class vec2_direction : uint8_t
    {
        Up,
        Left,
        Down,
        Right,
        None
    };

    /// @struct ivec2
    /// @brief  Vecteur à 2 dimensions avec des composantes de type \c int32_t.
    struct ivec2
    {
        int32_t x;    ///< Composante X du vecteur.
        int32_t y;    ///< Composante Y du vecteur.

        /// @brief   Constructeur de vecteurs à 2 dimensions de type \c int32_t.
        /// @param x Valeur de la composante X.
        /// @param y Valeur de la composante Y.
        DE_API ivec2(int32_t x = 0, int32_t y = 0);

        /// @brief          Trace le vecteur à partir de l'origine.
        /// @param renderer Le renderer qui s'occupe de dessiner la ligne.
        DE_API void draw(renderer &renderer) const;
    };

    /*
    ============
    ivec2::ivec2
    ============
    */
    inline ivec2::ivec2(int32_t _x, int32_t _y)
        : x(_x), y(_y)
    { }

    template<typename Type>
    struct vec2
    {
        Type x;
        Type y;

        vec2(const Type &x = Type(), const Type &y = Type());

        vec2 operator-(const vec2 &vec) const;

        static vec2 sub(const vec2 &vec1, const vec2 &vec2);
        static vec2 add(const vec2 &vec1, const vec2 &vec2);
        static vec2 mul(const vec2 &vec, const Type &value);
    };

    template<typename Type>
    vec2<Type>::vec2(const Type &_x, const Type &_y)
        : x(_x),
          y(_y)
    { }

    template<typename Type>
    vec2<Type> vec2<Type>::operator-(const vec2 &vec) const
    {
        return sub(*this, vec);
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
    struct vec3
    {
        Type x;
        Type y;
        Type z;

        vec3(const Type &x = Type(), const Type &y = Type(), const Type &z = Type());

        vec3 operator-(const vec3 &vec) const;
        vec3 &operator-=(const vec3 &vec);
        vec3 operator+(const vec3 &vec) const;
        vec3 &operator+=(const vec3 &vec);
        vec3 operator*(const Type &value) const;
        vec3 &operator*=(const Type &value);

        static vec3 sub(const vec3 &vec1, const vec3 &vec2);
        static vec3 add(const vec3 &vec1, const vec3 &vec2);
        static vec3 mul(const vec3 &vec, const Type &value);

        static float magn(const vec3 &vec);
        static vec3 normalize(const vec3 &vec);
        static vec3 cross(const vec3 &vec1, const vec3 &vec2);
        static float dot(const vec3 &vec1, const vec3 &vec2);
    };

    template<typename Type>
    vec3<Type>::vec3(const Type &_x, const Type &_y, const Type &_z)
        : x(_x),
          y(_y),
          z(_z)
    { }

    template<typename Type>
    vec3<Type> vec3<Type>::operator-(const vec3 &vec) const
    {
        return sub(*this, vec);
    }

    template<typename Type>
    vec3<Type> &vec3<Type>::operator-=(const vec3 &vec)
    {
        *this = sub(*this, vec);

        return *this;
    }

    template<typename Type>
    vec3<Type> vec3<Type>::operator+(const vec3 &vec) const
    {
        return add(*this, vec);
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
    struct vec4
    {
        Type x;
        Type y;
        Type z;
        Type w;

        vec4(const Type &x = Type(), const Type &y = Type(), const Type &z = Type(), const Type &w = Type());

        vec3<Type> xyz() const;
        vec3<Type> rgb() const;
        vec3<Type> bgr() const;
    };

    template<typename Type>
    vec4<Type>::vec4(const Type &_x, const Type &_y, const Type &_z, const Type &_w)
        : x(_x),
          y(_y),
          z(_z),
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