#ifndef __DEEP_ENGINE_MAT_IMPL_HPP__
#define __DEEP_ENGINE_MAT_IMPL_HPP__

#include "maths/vec.hpp"
#include "maths/mat.hpp"

namespace deep
{

    /*
    ==============
    mat2x2::mat2x2
    ==============
    */
    template<typename Type>
    inline mat2x2<Type>::mat2x2(const Type &x1, const Type &y1, const Type &x2, const Type &y2)
    {
        data[to_utype(index::x1)] = x1; data[to_utype(index::y1)] = y1;
        data[to_utype(index::x2)] = x2; data[to_utype(index::y2)] = y2;
    }

    /*
    ==================
    mat2x2::operator[]
    ==================
    */
    template<typename Type>
    inline Type mat2x2<Type>::operator[](size_t index) const
    {
        return data[index];
    }

    /*
    ==================
    mat2x2::operator[]
    ==================
    */
    template<typename Type>
    inline Type &mat2x2<Type>::operator[](size_t index)
    {
        return data[index];
    }

    /*
    =================
    mat2x2::operator*
    =================
    */
    template<typename Type>
    inline vec2<Type> mat2x2<Type>::operator*(const vec2<Type> &vec) const
    {
        return mul(*this, vec);
    }

    /*
    ===========
    mat2x2::mul
    ===========
    */
    template<typename Type>
    inline vec2<Type> mat2x2<Type>::mul(const mat2x2 &mat, const vec2<Type> &vec)
    {
        return vec2<Type>::mul(vec, mat);
    }

    /*
    =============
    mat2x2::scale
    =============
    */
    template<typename Type>
    inline vec2<Type> mat2x2<Type>::scale(const vec2<Type> &vec, const Type &scale1, const Type &scale2)
    {
        return vec2<Type>::mul(vec, {
                                        scale1,               static_cast<Type>(0),
                                        static_cast<Type>(0), scale2 
                                    } );
    }

    /*
    =================
    mat2x2::translate
    =================
    */
    template<typename Type>
    inline vec2<Type> mat2x2<Type>::translate(const vec2<Type> &vec, const Type &x, const Type &y)
    {
        return
        {
            vec.x + x, vec.y + y
        };
    }

    /*
    ==============
    mat2x2::rotate
    ==============
    */
    template<typename Type>
    inline vec2<Type> mat2x2<Type>::rotate(const vec2<Type> &vec, const Type &degrees)
    {
        float rad = deg_to_rad(static_cast<float>(degrees));

        return vec2<Type>::mul(vec, {
            cosf(rad), -sinf(rad),
            sinf(rad), cosf(rad) 
        } );
    }

    /*
    ===================
    mat2x2::determinant
    ===================
    */
    template<typename Type>
    inline Type mat2x2<Type>::determinant(const mat2x2 &mat)
    {
        return mat[0] * mat[3] - mat[1] * mat[2];
    }

    /*
    ==============
    mat3x3::mat3x3
    ==============
    */
    template<typename Type>
    mat3x3<Type>::mat3x3(
            const Type &x1, const Type &y1, const Type &z1,
            const Type &x2, const Type &y2, const Type &z2,
            const Type &x3, const Type &y3, const Type &z3
    )
    { 
        data[0] = x1; data[1] = y1; data[2] = z1;
        data[3] = x2; data[4] = y2; data[5] = z2;
        data[6] = x3; data[7] = y3; data[8] = z3;
    }

    /*
    =================
    mat3x3::operator*
    =================
    */
    template<typename Type>
    inline vec3<Type> mat3x3<Type>::operator*(const vec3<Type> &vec) const
    {
        return mul(*this, vec);
    }

    /*
    =================
    mat3x3::operator*
    =================
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::operator*(const mat3x3 &mat) const
    {
        return mul(*this, mat);
    }

    /*
    ==================
    mat3x3::operator[]
    ==================
    */
    template<typename Type>
    inline Type mat3x3<Type>::operator[](size_t index) const
    {
        return data[index];
    }

    /*
    ===========
    mat3x3::ptr
    ===========
    */
    template<typename Type>
    inline Type *mat3x3<Type>::ptr() const
    {
        return rm_const<Type *>(data);
    }

    /*
    ===========
    mat3x3::mul
    ===========
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::mul(const mat3x3 &mat, const Type &value)
    {
        return
        {
            value * mat[to_utype(index::x1)], value * mat[to_utype(index::y1)], value * mat[to_utype(index::z1)],
            value * mat[to_utype(index::x2)], value * mat[to_utype(index::y2)], value * mat[to_utype(index::z2)],
            value * mat[to_utype(index::x3)], value * mat[to_utype(index::y3)], value * mat[to_utype(index::z3)]
        };
    }

    /*
    ===========
    mat3x3::mul
    ===========
    */
    template<typename Type>
    inline vec3<Type> mat3x3<Type>::mul(const mat3x3 &mat, const vec3<Type> &vec)
    {
        return vec3<Type>::mul(vec, mat);
    }

    /*
    ===========
    mat3x3::mul
    ===========
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::mul(const mat3x3 &mat1, const mat3x3 &mat2)
    {
        return
        {
            (mat1[to_utype(index::x1)] * mat2[to_utype(index::x1)]) + 
            (mat1[to_utype(index::y1)] * mat2[to_utype(index::x2)]) + 
            (mat1[to_utype(index::z1)] * mat2[to_utype(index::x3)]), 
            (mat1[to_utype(index::x1)] * mat2[to_utype(index::y1)]) + 
            (mat1[to_utype(index::y1)] * mat2[to_utype(index::y2)]) + 
            (mat1[to_utype(index::z1)] * mat2[to_utype(index::y3)]), 
            (mat1[to_utype(index::x1)] * mat2[to_utype(index::z1)]) + 
            (mat1[to_utype(index::y1)] * mat2[to_utype(index::z2)]) + 
            (mat1[to_utype(index::z1)] * mat2[to_utype(index::z3)]),
            (mat1[to_utype(index::x2)] * mat2[to_utype(index::x1)]) + 
            (mat1[to_utype(index::y2)] * mat2[to_utype(index::x2)]) + 
            (mat1[to_utype(index::z2)] * mat2[to_utype(index::x3)]), 
            (mat1[to_utype(index::x2)] * mat2[to_utype(index::y1)]) + 
            (mat1[to_utype(index::y2)] * mat2[to_utype(index::y2)]) + 
            (mat1[to_utype(index::z2)] * mat2[to_utype(index::y3)]), 
            (mat1[to_utype(index::x2)] * mat2[to_utype(index::z1)]) + 
            (mat1[to_utype(index::y2)] * mat2[to_utype(index::z2)]) + 
            (mat1[to_utype(index::z2)] * mat2[to_utype(index::z3)]),
            (mat1[to_utype(index::x3)] * mat2[to_utype(index::x1)]) + 
            (mat1[to_utype(index::y3)] * mat2[to_utype(index::x2)]) + 
            (mat1[to_utype(index::z3)] * mat2[to_utype(index::x3)]), 
            (mat1[to_utype(index::x3)] * mat2[to_utype(index::y1)]) + 
            (mat1[to_utype(index::y3)] * mat2[to_utype(index::y2)]) + 
            (mat1[to_utype(index::z3)] * mat2[to_utype(index::y3)]), 
            (mat1[to_utype(index::x3)] * mat2[to_utype(index::z1)]) + 
            (mat1[to_utype(index::y3)] * mat2[to_utype(index::z2)]) + 
            (mat1[to_utype(index::z3)] * mat2[to_utype(index::z3)])
        };
    }

    /*
    ===========
    mat3x3::add
    ===========
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::add(const mat3x3 &mat1, const mat3x3 &mat2)
    {
        return
        {
            mat1[to_utype(index::x1)] + mat2[to_utype(index::x1)], mat1[to_utype(index::y1)] + mat2[to_utype(index::y1)], mat1[to_utype(index::z1)] + mat2[to_utype(index::z1)],
            mat1[to_utype(index::x2)] + mat2[to_utype(index::x2)], mat1[to_utype(index::y2)] + mat2[to_utype(index::y2)], mat1[to_utype(index::z2)] + mat2[to_utype(index::z2)],
            mat1[to_utype(index::x3)] + mat2[to_utype(index::x3)], mat1[to_utype(index::y3)] + mat2[to_utype(index::y3)], mat1[to_utype(index::z3)] + mat2[to_utype(index::z3)]
        };
    }

    /*
    ===================
    mat3x3::determinant
    ===================
    */
    template<typename Type>
    inline Type mat3x3<Type>::determinant(const mat3x3 &mat)
    {
        mat2x2<Type> a = mat2x2(
            mat[to_utype(index::y2)], mat[to_utype(index::z2)],
            mat[to_utype(index::y3)], mat[to_utype(index::z3)]
        );

        mat2x2<Type> b = mat2x2(
            mat[to_utype(index::x2)], mat[to_utype(index::z2)],
            mat[to_utype(index::x3)], mat[to_utype(index::z3)]
        );

        mat2x2<Type> c = mat2x2(
            mat[to_utype(index::x2)], mat[to_utype(index::y2)],
            mat[to_utype(index::x3)], mat[to_utype(index::y3)]
        );

        return
            mat[to_utype(index::x1)] * mat2x2<Type>::determinant(a) -
            mat[to_utype(index::y1)] * mat2x2<Type>::determinant(b) +
            mat[to_utype(index::z1)] * mat2x2<Type>::determinant(c);
    }

    /*
    =================
    mat3x3::translate
    =================
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::translate(const mat3x3 &mat, const vec2<Type> &vec)
    {
        return mul(
            mat,
            {
                static_cast<Type>(1), static_cast<Type>(0), vec.x,
                static_cast<Type>(0), static_cast<Type>(1), vec.y,
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    =============
    mat3x3::scale
    =============
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::scale(const mat3x3 &mat, const vec2<Type> &vec)
    {
        return mul(
            mat,
            {
                vec.x,                static_cast<Type>(0), static_cast<Type>(0),
                static_cast<Type>(0), vec.y,                static_cast<Type>(0),
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    =============
    mat3x3::scale
    =============
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::scale(const mat3x3 &mat, const Type &value)
    {
        return mul(mat, value);
    }

    /*
    ==============
    mat3x3::rotate
    ==============
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::rotate(const mat3x3 &mat, const Type &degrees)
    {
        float rad = deg_to_rad(static_cast<float>(degrees));

        return mul(
            mat,
            {
                cosf(rad),            -sinf(rad),            static_cast<Type>(0),
                sinf(rad),             cosf(rad),            static_cast<Type>(0),
                static_cast<Type>(0),  static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    ==============
    mat4x4::mat4x4
    ==============
    */
    template<typename Type>
    inline mat4x4<Type>::mat4x4(
            const Type &x1, const Type &y1, const Type &z1, const Type &w1,
            const Type &x2, const Type &y2, const Type &z2, const Type &w2,
            const Type &x3, const Type &y3, const Type &z3, const Type &w3,
            const Type &x4, const Type &y4, const Type &z4, const Type &w4
    )
    {
        data[to_utype(index::x1)] = x1; data[to_utype(index::y1)] = y1; data[to_utype(index::z1)] = z1; data[to_utype(index::w1)] = w1;
        data[to_utype(index::x2)] = x2; data[to_utype(index::y2)] = y2; data[to_utype(index::z2)] = z2; data[to_utype(index::w2)] = w2;
        data[to_utype(index::x3)] = x3; data[to_utype(index::y3)] = y3; data[to_utype(index::z3)] = z3; data[to_utype(index::w3)] = w3;
        data[to_utype(index::x4)] = x4; data[to_utype(index::y4)] = y4; data[to_utype(index::z4)] = z4; data[to_utype(index::w4)] = w4;
    }

    /*
    ===========
    mat4x4::mul
    ===========
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::mul(const mat4x4 &mat1, const mat4x4 &mat2)
    {
        return
        {
            (
                mat1[to_utype(index::x1)] * 
                mat2[to_utype(index::x1)] + 
                mat1[to_utype(index::y1)] * 
                mat2[to_utype(index::x2)] + 
                mat1[to_utype(index::z1)] * 
                mat2[to_utype(index::x3)] + 
                mat1[to_utype(index::w1)] * 
                mat2[to_utype(index::x4)]
            ),
            (
                mat1[to_utype(index::x1)] * 
                mat2[to_utype(index::y1)] + 
                mat1[to_utype(index::y1)] * 
                mat2[to_utype(index::y2)] + 
                mat1[to_utype(index::z1)] * 
                mat2[to_utype(index::y3)] + 
                mat1[to_utype(index::w1)] * 
                mat2[to_utype(index::y4)]
            ), 
            (
                mat1[to_utype(index::x1)] * 
                mat2[to_utype(index::z1)] + 
                mat1[to_utype(index::y1)] * 
                mat2[to_utype(index::z2)] + 
                mat1[to_utype(index::z1)] * 
                mat2[to_utype(index::z3)] + 
                mat1[to_utype(index::w1)] * 
                mat2[to_utype(index::z4)]
            ), 
            (
                mat1[to_utype(index::x1)] * 
                mat2[to_utype(index::w1)] + 
                mat1[to_utype(index::y1)] * 
                mat2[to_utype(index::w2)] + 
                mat1[to_utype(index::z1)] * 
                mat2[to_utype(index::w3)] + 
                mat1[to_utype(index::w1)] * 
                mat2[to_utype(index::w4)]
            ),

            (
                mat1[to_utype(index::x2)] * 
                mat2[to_utype(index::x1)] + 
                mat1[to_utype(index::y2)] * 
                mat2[to_utype(index::x2)] + 
                mat1[to_utype(index::z2)] * 
                mat2[to_utype(index::x3)] + 
                mat1[to_utype(index::w2)] * 
                mat2[to_utype(index::x4)]
            ), 
            (
                mat1[to_utype(index::x2)] * 
                mat2[to_utype(index::y1)] + 
                mat1[to_utype(index::y2)] * 
                mat2[to_utype(index::y2)] + 
                mat1[to_utype(index::z2)] * 
                mat2[to_utype(index::y3)] + 
                mat1[to_utype(index::w2)] * 
                mat2[to_utype(index::y4)]
            ), 
            (
                mat1[to_utype(index::x2)] * 
                mat2[to_utype(index::z1)] + 
                mat1[to_utype(index::y2)] * 
                mat2[to_utype(index::z2)] + 
                mat1[to_utype(index::z2)] * 
                mat2[to_utype(index::z3)] + 
                mat1[to_utype(index::w2)] * 
                mat2[to_utype(index::z4)]
            ), 
            (
                mat1[to_utype(index::x2)] * 
                mat2[to_utype(index::w1)] + 
                mat1[to_utype(index::y2)] * 
                mat2[to_utype(index::w2)] + 
                mat1[to_utype(index::z2)] * 
                mat2[to_utype(index::w3)] + 
                mat1[to_utype(index::w2)] * 
                mat2[to_utype(index::w4)]
            ),
            (
                mat1[to_utype(index::x3)] * 
                mat2[to_utype(index::x1)] + 
                mat1[to_utype(index::y3)] * 
                mat2[to_utype(index::x2)] + 
                mat1[to_utype(index::z3)] * 
                mat2[to_utype(index::x3)] + 
                mat1[to_utype(index::w3)] * 
                mat2[to_utype(index::x4)]
            ), 
            (
                mat1[to_utype(index::x3)] * 
                mat2[to_utype(index::y1)] + 
                mat1[to_utype(index::y3)] * 
                mat2[to_utype(index::y2)] + 
                mat1[to_utype(index::z3)] * 
                mat2[to_utype(index::y3)] + 
                mat1[to_utype(index::w3)] * 
                mat2[to_utype(index::y4)]
            ), 
            (
                mat1[to_utype(index::x3)] * 
                mat2[to_utype(index::z1)] + 
                mat1[to_utype(index::y3)] * 
                mat2[to_utype(index::z2)] + 
                mat1[to_utype(index::z3)] * 
                mat2[to_utype(index::z3)] + 
                mat1[to_utype(index::w3)] * 
                mat2[to_utype(index::z4)]
            ), 
            (
                mat1[to_utype(index::x3)] * 
                mat2[to_utype(index::w1)] + 
                mat1[to_utype(index::y3)] * 
                mat2[to_utype(index::w2)] + 
                mat1[to_utype(index::z3)] * 
                mat2[to_utype(index::w3)] + 
                mat1[to_utype(index::w3)] * 
                mat2[to_utype(index::w4)]
            ),
            (
                mat1[to_utype(index::x4)] * 
                mat2[to_utype(index::x1)] + 
                mat1[to_utype(index::y4)] * 
                mat2[to_utype(index::x2)] + 
                mat1[to_utype(index::z4)] * 
                mat2[to_utype(index::x3)] + 
                mat1[to_utype(index::w4)] * 
                mat2[to_utype(index::x4)]
            ), 
            (
                mat1[to_utype(index::x4)] * 
                mat2[to_utype(index::y1)] + 
                mat1[to_utype(index::y4)] * 
                mat2[to_utype(index::y2)] + 
                mat1[to_utype(index::z4)] * 
                mat2[to_utype(index::y3)] + 
                mat1[to_utype(index::w4)] * 
                mat2[to_utype(index::y4)]
            ),
            (
                mat1[to_utype(index::x4)] * 
                mat2[to_utype(index::z1)] + 
                mat1[to_utype(index::y4)] * 
                mat2[to_utype(index::z2)] + 
                mat1[to_utype(index::z4)] * 
                mat2[to_utype(index::z3)] + 
                mat1[to_utype(index::w4)] * 
                mat2[to_utype(index::z4)]
            ), 
            (
                mat1[to_utype(index::x4)] *
                mat2[to_utype(index::w1)] + 
                mat1[to_utype(index::y4)] * 
                mat2[to_utype(index::w2)] + 
                mat1[to_utype(index::z4)] * 
                mat2[to_utype(index::w3)] + 
                mat1[to_utype(index::w4)] * 
                mat2[to_utype(index::w4)]
            )
        };
    }

    /*
    =================
    mat4x4::operator*
    =================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::operator*(const mat4x4 &mat) const
    {
        return mul(*this, mat);
    }

    /*
    ==================
    mat4x4::operator[]
    ==================
    */
    template<typename Type>
    inline Type mat4x4<Type>::operator[](size_t index) const
    {
        return data[index];
    }

    /*
    ==================
    mat4x4::operator[]
    ==================
    */
    template<typename Type>
    inline Type &mat4x4<Type>::operator[](size_t index)
    {
        return data[index];
    }

    /*
    ===========
    mat4x4::ptr
    ===========
    */
    template<typename Type>
    inline Type *mat4x4<Type>::ptr() const
    {
        return rm_const<Type *>(data);
    }

    /*
    =================
    mat4x4::translate
    =================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::translate(const mat4x4 &mat, const vec3<Type> &vec)
    {
        return mul(
            mat,
            {
                static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0), vec.x,
                static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0), vec.y,
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1), vec.z,
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    =============
    mat4x4::scale
    =============
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::scale(const mat4x4 &mat, const vec3<Type> &vec)
    {
        return mul(
            mat,
            {
                vec.x,                 static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0),
                static_cast<Type>(0),  vec.y,                static_cast<Type>(0), static_cast<Type>(0),
                static_cast<Type>(0),  static_cast<Type>(0), vec.z,                static_cast<Type>(0),
                static_cast<Type>(0),  static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    ================
    mat4x4::rotate_x
    ================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::rotate_x(const mat4x4 &mat, const Type &degrees)
    {
        float rad = deg_to_rad(static_cast<float>(degrees));

        return mul(
            mat,
            {
                static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0),
                static_cast<Type>(0), cosf(rad),           -sinf(rad),            static_cast<Type>(0),
                static_cast<Type>(0), sinf(rad),            cosf(rad),            static_cast<Type>(0),
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    ================
    mat4x4::rotate_y
    ================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::rotate_y(const mat4x4 &mat, const Type &degrees)
    {
        float rad = deg_to_rad(static_cast<float>(degrees));

        return mul(
            mat,
            {
                 cosf(rad),            static_cast<Type>(0), sinf(rad),            static_cast<Type>(0),
                 static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0),
                -sinf(rad),            static_cast<Type>(0), cosf(rad),            static_cast<Type>(0),
                 static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    ================
    mat4x4::rotate_z
    ================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::rotate_z(const mat4x4 &mat, const Type &degrees)
    {
        float rad = deg_to_rad(static_cast<float>(degrees));

        return mul(
            mat,
            {
                cosf(rad),           -sinf(rad),            static_cast<Type>(0), static_cast<Type>(0),
                sinf(rad),            cosf(rad),            static_cast<Type>(0), static_cast<Type>(0),
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0),
                static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
            }
        );
    }

    /*
    ===================
    mat4x4::perspective
    ===================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::perspective(const mat4x4 &mat, const Type &fovy, const Type &aspectRatio, const Type &znear, const Type &zfar)
    {
        Type tanHalfFovy = tanf(static_cast<float>(fovy / static_cast<Type>(2)));
        Type zr = zfar - znear;

        return
        {
            mat[to_utype(index::x1)] / (aspectRatio * tanHalfFovy), static_cast<Type>(0),                   static_cast<Type>(0),  static_cast<Type>(0),
            static_cast<Type>(0),                                   mat[to_utype(index::y2)] / tanHalfFovy, static_cast<Type>(0),  static_cast<Type>(0),
            static_cast<Type>(0),                                   static_cast<Type>(0),                 -(zfar + znear) / zr,  -(static_cast<Type>(2) * zfar * znear) / zr,
            static_cast<Type>(0),                                   static_cast<Type>(0),                   static_cast<Type>(-1), static_cast<Type>(0)
        };
    }
    

    /*
    ===================
    mat4x4::determinant
    ===================
    */
    template<typename Type>
    inline Type mat4x4<Type>::determinant(const mat4x4 &mat)
    {
        mat3x3<Type> a = mat3x3(
            mat[to_utype(index::y2)], mat[to_utype(index::z2)], mat[to_utype(index::w2)], 
            mat[to_utype(index::y3)], mat[to_utype(index::z3)], mat[to_utype(index::w3)], 
            mat[to_utype(index::y4)], mat[to_utype(index::z4)], mat[to_utype(index::w4)]
        );

        mat3x3<Type> b = mat3x3(
            mat[to_utype(index::x2)], mat[to_utype(index::z2)], mat[to_utype(index::w2)], 
            mat[to_utype(index::x3)], mat[to_utype(index::z3)], mat[to_utype(index::w3)], 
            mat[to_utype(index::x4)], mat[to_utype(index::z4)], mat[to_utype(index::w4)]
        );

        mat3x3<Type> c = mat3x3(
            mat[to_utype(index::x2)], mat[to_utype(index::y2)], mat[to_utype(index::w2)], 
            mat[to_utype(index::x3)], mat[to_utype(index::y3)], mat[to_utype(index::w3)], 
            mat[to_utype(index::x4)], mat[to_utype(index::y4)], mat[to_utype(index::w4)]
        );

        mat3x3<float> d = mat3x3(
            mat[to_utype(index::x2)], mat[to_utype(index::y2)], mat[to_utype(index::z2)], 
            mat[to_utype(index::x3)], mat[to_utype(index::y3)], mat[to_utype(index::z3)], 
            mat[to_utype(index::x4)], mat[to_utype(index::y4)], mat[to_utype(index::z4)]
        );

        return
            mat[to_utype(index::x1)] * mat3x3<Type>::determinant(a) -
            mat[to_utype(index::y1)] * mat3x3<Type>::determinant(b) +
            mat[to_utype(index::z1)] * mat3x3<Type>::determinant(c) -
            mat[to_utype(index::w1)] * mat3x3<Type>::determinant(d);
    }

}


#endif