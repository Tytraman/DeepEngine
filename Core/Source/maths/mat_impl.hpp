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
    =================
    mat2x2::operator=
    =================
    */
    template<typename Type>
    inline mat2x2<Type> &mat2x2<Type>::operator=(const mat2x2 &other)
    {
        data[to_utype(index::x1)] = other[to_utype(index::x1)]; data[to_utype(index::y1)] = other[to_utype(index::y1)];
        data[to_utype(index::x2)] = other[to_utype(index::x2)]; data[to_utype(index::y2)] = other[to_utype(index::y2)];

        return *this;
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
    =================
    mat2x2::operator*
    =================
    */
    template<typename Type>
    inline mat2x2<Type> mat2x2<Type>::operator*(const Type &value) const
    {
        return mul(*this, value);
    }

    /*
    ==================
    mat2x2::operator==
    ==================
    */
    template<typename Type>
    inline bool mat2x2<Type>::operator==(const mat2x2 &other) const
    {
        return memcmp(data, other.data, sizeof(data)) == 0;
    }

    /*
    ===========
    mat2x2::mul
    ===========
    */
    template<typename Type>
    inline mat2x2<Type> mat2x2<Type>::mul(const mat2x2 &mat, const Type &value)
    {
        return
        {
            value * mat[to_utype(index::x1)], value * mat[to_utype(index::y1)],
            value * mat[to_utype(index::x2)], value * mat[to_utype(index::y2)]
        };
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
    ===============
    mat2x2::inverse
    ===============
    */
    template<typename Type>
    inline bool mat2x2<Type>::inverse(const mat2x2 &mat, mat2x2 &dest)
    {
        mat2x2<Type> m =
        {
             mat[to_utype(index::y2)], -mat[to_utype(index::y1)],
            -mat[to_utype(index::x2)],  mat[to_utype(index::x1)]
        };

        Type det = determinant(mat);

        if(det == static_cast<Type>(0))
        {
            return false;
        }

        Type v = static_cast<Type>(1) /
                  det;

        dest = m * v;

        return true;
    }

    /*
    =================
    mat2x2::transpose
    =================
    */
    template<typename Type>
    inline mat2x2<Type> mat2x2<Type>::transpose(const mat2x2 &mat)
    {
        return
        {
            mat[index::x1], mat[index::x2],
            mat[index::y1], mat[index::y2]
        };
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
        data[to_utype(index::x1)] = x1; data[to_utype(index::y1)] = y1; data[to_utype(index::z1)] = z1;
        data[to_utype(index::x2)] = x2; data[to_utype(index::y2)] = y2; data[to_utype(index::z2)] = z2;
        data[to_utype(index::x3)] = x3; data[to_utype(index::y3)] = y3; data[to_utype(index::z3)] = z3;
    }

    /*
    =================
    mat3x3::operator=
    =================
    */
    template<typename Type>
    mat3x3<Type> &mat3x3<Type>::operator=(const mat3x3 &other)
    {
        data[to_utype(index::x1)] = other[to_utype(index::x1)]; data[to_utype(index::y1)] = other[to_utype(index::y1)]; data[to_utype(index::z1)] = other[to_utype(index::z1)];
        data[to_utype(index::x2)] = other[to_utype(index::x2)]; data[to_utype(index::y2)] = other[to_utype(index::y2)]; data[to_utype(index::z2)] = other[to_utype(index::z2)];
        data[to_utype(index::x3)] = other[to_utype(index::x3)]; data[to_utype(index::y3)] = other[to_utype(index::y3)]; data[to_utype(index::z3)] = other[to_utype(index::z3)];

        return *this;
    }

    /*
    =================
    mat3x3::operator*
    =================
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::operator*(const Type &value) const
    {
        return mul(*this, value);
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
    ==================
    mat3x3::operator==
    ==================
    */
    template<typename Type>
    inline bool mat3x3<Type>::operator==(const mat3x3 &other) const
    {
        return memcmp(data, other.data, sizeof(data)) == 0;
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
    ===============
    mat3x3::inverse
    ===============
    */
    template<typename Type>
    inline bool mat3x3<Type>::inverse(const mat3x3 &mat, mat3x3 &dest)
    {
        Type det = determinant(mat);

        if(det == static_cast<Type>(0))
        {
            return false;
        }

        // Matrix of Minors -> Cofactors

        mat2x2<Type> m11 =
        {
            mat[to_utype(index::y2)], mat[to_utype(index::z2)],
            mat[to_utype(index::y3)], mat[to_utype(index::z3)]
        };

        mat2x2<Type> m12 =
        {
            mat[to_utype(index::x2)], mat[to_utype(index::z2)],
            mat[to_utype(index::x3)], mat[to_utype(index::z3)]
        };

        mat2x2<Type> m13 =
        {
            mat[to_utype(index::x2)], mat[to_utype(index::y2)],
            mat[to_utype(index::x3)], mat[to_utype(index::y3)]
        };

        mat2x2<Type> m21 =
        {
            mat[to_utype(index::y1)], mat[to_utype(index::z1)],
            mat[to_utype(index::y3)], mat[to_utype(index::z3)]
        };

        mat2x2<Type> m22 =
        {
            mat[to_utype(index::x1)], mat[to_utype(index::z1)],
            mat[to_utype(index::x3)], mat[to_utype(index::z3)]
        };

        mat2x2<Type> m23 =
        {
            mat[to_utype(index::x1)], mat[to_utype(index::y1)],
            mat[to_utype(index::x3)], mat[to_utype(index::y3)]
        };

        mat2x2<Type> m31 =
        {
            mat[to_utype(index::y1)], mat[to_utype(index::z1)],
            mat[to_utype(index::y2)], mat[to_utype(index::z2)]
        };

        mat2x2<Type> m32 =
        {
            mat[to_utype(index::x1)], mat[to_utype(index::z1)],
            mat[to_utype(index::x2)], mat[to_utype(index::z2)]
        };

        mat2x2<Type> m33 =
        {
            mat[to_utype(index::x1)], mat[to_utype(index::y1)],
            mat[to_utype(index::x2)], mat[to_utype(index::y2)]
        };

        mat3x3<Type> cofactors =
        {
            mat2x2<Type>::determinant(m11), -mat2x2<Type>::determinant(m12), mat2x2<Type>::determinant(m13),
            -mat2x2<Type>::determinant(m21), mat2x2<Type>::determinant(m22), -mat2x2<Type>::determinant(m23),
            mat2x2<Type>::determinant(m31), -mat2x2<Type>::determinant(m32), mat2x2<Type>::determinant(m33)
        };

        cofactors = transpose(cofactors);

        dest = cofactors * (static_cast<Type>(1) / det);

        return true;
    }

    template<typename Type>
    inline bool mat4x4<Type>::inverse(const mat4x4 &mat, mat4x4 &dest)
    {
        Type det = determinant(mat);

        if(det == static_cast<Type>(0))
        {
            return false;
        }

        mat4x4_row<Type> rightRow1 = { static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0) };
        mat4x4_row<Type> rightRow2 = { static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0) };
        mat4x4_row<Type> rightRow3 = { static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0) };
        mat4x4_row<Type> rightRow4 = { static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1) };

        mat4x4_row<Type> leftRow1 = mat.get_row_1();
        mat4x4_row<Type> leftRow2 = mat.get_row_2();
        mat4x4_row<Type> leftRow3 = mat.get_row_3();
        mat4x4_row<Type> leftRow4 = mat.get_row_4();

        mat4x4_row<Type> pivotLeftRow1 = leftRow1;
        mat4x4_row<Type> pivotRightRow1 = rightRow1;
        make_pivot_row(0, pivotLeftRow1, pivotRightRow1);
        leftRow1 = pivotLeftRow1;
        rightRow1 = pivotRightRow1;

        calc_inverse_row(0, pivotLeftRow1, pivotRightRow1, leftRow2, rightRow2);
        calc_inverse_row(0, pivotLeftRow1, pivotRightRow1, leftRow3, rightRow3);
        calc_inverse_row(0, pivotLeftRow1, pivotRightRow1, leftRow4, rightRow4);

        mat4x4_row<Type> pivotLeftRow2 = leftRow2;
        mat4x4_row<Type> pivotRightRow2 = rightRow2;
        make_pivot_row(1, pivotLeftRow2, pivotRightRow2);
        leftRow2 = pivotLeftRow2;
        rightRow2 = pivotRightRow2;

        calc_inverse_row(1, pivotLeftRow2, pivotRightRow2, leftRow1, rightRow1);
        calc_inverse_row(1, pivotLeftRow2, pivotRightRow2, leftRow3, rightRow3);
        calc_inverse_row(1, pivotLeftRow2, pivotRightRow2, leftRow4, rightRow4);

        mat4x4_row<Type> pivotLeftRow3 = leftRow3;
        mat4x4_row<Type> pivotRightRow3 = rightRow3;
        make_pivot_row(2, pivotLeftRow3, pivotRightRow3);
        leftRow3 = pivotLeftRow3;
        rightRow3 = pivotRightRow3;

        calc_inverse_row(2, pivotLeftRow3, pivotRightRow3, leftRow1, rightRow1);
        calc_inverse_row(2, pivotLeftRow3, pivotRightRow3, leftRow2, rightRow2);
        calc_inverse_row(2, pivotLeftRow3, pivotRightRow3, leftRow4, rightRow4);

        mat4x4_row<Type> pivotLeftRow4 = leftRow4;
        mat4x4_row<Type> pivotRightRow4 = rightRow4;
        make_pivot_row(3, pivotLeftRow4, pivotRightRow4);
        leftRow4 = pivotLeftRow4;
        rightRow4 = pivotRightRow4;
         
        calc_inverse_row(3, pivotLeftRow4, pivotRightRow4, leftRow1, rightRow1);
        calc_inverse_row(3, pivotLeftRow4, pivotRightRow4, leftRow2, rightRow2);
        calc_inverse_row(3, pivotLeftRow4, pivotRightRow4, leftRow3, rightRow3);


        dest =
        {
            rightRow1,
            rightRow2,
            rightRow3,
            rightRow4
        };

        return true;
    }

    /*
    =================
    mat3x3::transpose
    =================
    */
    template<typename Type>
    inline mat3x3<Type> mat3x3<Type>::transpose(const mat3x3 &mat)
    {
        return
        {
            mat[to_utype(index::x1)], mat[to_utype(index::x2)], mat[to_utype(index::x3)],
            mat[to_utype(index::y1)], mat[to_utype(index::y2)], mat[to_utype(index::y3)],
            mat[to_utype(index::z1)], mat[to_utype(index::z2)], mat[to_utype(index::z3)]
        };
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
    ======================
    mat4x4_row::mat4x4_row
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type>::mat4x4_row(const Type &x, const Type &y, const Type &z, const Type &w)
    {
        data[to_utype(index::x)] = x;
        data[to_utype(index::y)] = y;
        data[to_utype(index::z)] = z;
        data[to_utype(index::w)] = w;
    }

    /*
    ======================
    mat4x4_row::mat4x4_row
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type>::mat4x4_row(const mat4x4_row &other)
    {
        data[to_utype(index::x)] = other[to_utype(index::x)];
        data[to_utype(index::y)] = other[to_utype(index::y)];
        data[to_utype(index::z)] = other[to_utype(index::z)];
        data[to_utype(index::w)] = other[to_utype(index::w)];
    }

    /*
    =====================
    mat4x4_row::operator=
    =====================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator=(const mat4x4_row &other)
    {
        data[to_utype(index::x)] = other[to_utype(index::x)];
        data[to_utype(index::y)] = other[to_utype(index::y)];
        data[to_utype(index::z)] = other[to_utype(index::z)];
        data[to_utype(index::w)] = other[to_utype(index::w)];

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator[]
    ======================
    */
    template<typename Type>
    inline Type &mat4x4_row<Type>::operator[](size_t index) const
    {
        return rm_const<Type>(data[index]);
    }

    /*
    ======================
    mat4x4_row::operator*=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator*=(const Type &value)
    {
        data[to_utype(index::x)] *= value;
        data[to_utype(index::y)] *= value;
        data[to_utype(index::z)] *= value;
        data[to_utype(index::w)] *= value;

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator*=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator*=(const mat4x4_row &other)
    {
        data[to_utype(index::x)] *= other[to_utype(index::x)];
        data[to_utype(index::y)] *= other[to_utype(index::y)];
        data[to_utype(index::z)] *= other[to_utype(index::z)];
        data[to_utype(index::w)] *= other[to_utype(index::w)];

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator/=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator/=(const Type &value)
    {
        data[to_utype(index::x)] /= value;
        data[to_utype(index::y)] /= value;
        data[to_utype(index::z)] /= value;
        data[to_utype(index::w)] /= value;

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator/=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator/=(const mat4x4_row &other)
    {
        data[to_utype(index::x)] /= other[to_utype(index::x)];
        data[to_utype(index::y)] /= other[to_utype(index::y)];
        data[to_utype(index::z)] /= other[to_utype(index::z)];
        data[to_utype(index::w)] /= other[to_utype(index::w)];

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator+=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator+=(const Type &value)
    {
        data[to_utype(index::x)] += value;
        data[to_utype(index::y)] += value;
        data[to_utype(index::z)] += value;
        data[to_utype(index::w)] += value;

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator+=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator+=(const mat4x4_row &other)
    {
        data[to_utype(index::x)] += other[to_utype(index::x)];
        data[to_utype(index::y)] += other[to_utype(index::y)];
        data[to_utype(index::z)] += other[to_utype(index::z)];
        data[to_utype(index::w)] += other[to_utype(index::w)];

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator-=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator-=(const Type &value)
    {
        data[to_utype(index::x)] -= value;
        data[to_utype(index::y)] -= value;
        data[to_utype(index::z)] -= value;
        data[to_utype(index::w)] -= value;

        return *this;
    }

    /*
    ======================
    mat4x4_row::operator-=
    ======================
    */
    template<typename Type>
    inline mat4x4_row<Type> &mat4x4_row<Type>::operator-=(const mat4x4_row &other)
    {
        data[to_utype(index::x)] -= other[to_utype(index::x)];
        data[to_utype(index::y)] -= other[to_utype(index::y)];
        data[to_utype(index::z)] -= other[to_utype(index::z)];
        data[to_utype(index::w)] -= other[to_utype(index::w)];

        return *this;
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
    ==============
    mat4x4::mat4x4
    ==============
    */
    template<typename Type>
    inline mat4x4<Type>::mat4x4(const mat4x4_row<Type> &row)
    {
        data[to_utype(index::x1)] = row[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y1)] = row[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z1)] = row[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w1)] = row[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x2)] = row[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y2)] = row[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z2)] = row[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w2)] = row[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x3)] = row[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y3)] = row[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z3)] = row[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w3)] = row[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x4)] = row[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y4)] = row[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z4)] = row[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w4)] = row[to_utype(mat4x4_row<Type>::index::w)];
    }

    /*
    ==============
    mat4x4::mat4x4
    ==============
    */
    template<typename Type>
    inline mat4x4<Type>::mat4x4(
        const mat4x4_row<Type> &row1,
        const mat4x4_row<Type> &row2,
        const mat4x4_row<Type> &row3,
        const mat4x4_row<Type> &row4
    )
    {
        data[to_utype(index::x1)] = row1[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y1)] = row1[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z1)] = row1[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w1)] = row1[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x2)] = row2[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y2)] = row2[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z2)] = row2[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w2)] = row2[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x3)] = row3[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y3)] = row3[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z3)] = row3[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w3)] = row3[to_utype(mat4x4_row<Type>::index::w)];
        data[to_utype(index::x4)] = row4[to_utype(mat4x4_row<Type>::index::x)]; data[to_utype(index::y4)] = row4[to_utype(mat4x4_row<Type>::index::y)]; data[to_utype(index::z4)] = row4[to_utype(mat4x4_row<Type>::index::z)]; data[to_utype(index::w4)] = row4[to_utype(mat4x4_row<Type>::index::w)];
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
    ==================
    mat4x4::operator==
    ==================
    */
    template<typename Type>
    bool mat4x4<Type>::operator==(const mat4x4 &other) const
    {
        return memcmp(data, other.data, sizeof(data)) == 0;
    }

    /*
    ======================
    mat4x4::make_pivot_row
    ======================
    */
    template<typename Type>
    inline void mat4x4<Type>::make_pivot_row(uint8_t pivotIndex, mat4x4_row<Type> &leftRow, mat4x4_row<Type> &rightRow)
    {
        Type temp = leftRow[pivotIndex];

        if(temp != static_cast<Type>(1))
        {
            leftRow /= temp;
            rightRow /= temp;
        }
    }

    /*
    ========================
    mat4x4::calc_inverse_row
    ========================
    */
    template<typename Type>
    inline void mat4x4<Type>::calc_inverse_row(uint8_t pivotIndex, mat4x4_row<Type> fromLeftRow, mat4x4_row<Type> fromRightRow, mat4x4_row<Type> &toLeftRow, mat4x4_row<Type> &toRightRow)
    {
        Type m = toLeftRow[pivotIndex];

        fromLeftRow *= m;
        fromRightRow *= m;

        toLeftRow -= fromLeftRow;
        toRightRow -= fromRightRow;
    }

    /*
    =================
    mat4x4::get_row_1
    =================
    */
    template<typename Type>
    inline mat4x4_row<Type> mat4x4<Type>::get_row_1() const
    {
        return
        {
            data[to_utype(index::x1)], data[to_utype(index::y1)], data[to_utype(index::z1)], data[to_utype(index::w1)]
        };
    }

    /*
    =================
    mat4x4::get_row_2
    =================
    */
    template<typename Type>
    inline mat4x4_row<Type> mat4x4<Type>::get_row_2() const
    {
        return
        {
            data[to_utype(index::x2)], data[to_utype(index::y2)], data[to_utype(index::z2)], data[to_utype(index::w2)]
        };
    }

    /*
    =================
    mat4x4::get_row_3
    =================
    */
    template<typename Type>
    inline mat4x4_row<Type> mat4x4<Type>::get_row_3() const
    {
        return
        {
            data[to_utype(index::x3)], data[to_utype(index::y3)], data[to_utype(index::z3)], data[to_utype(index::w3)]
        };
    }

    /*
    =================
    mat4x4::get_row_4
    =================
    */
    template<typename Type>
    inline mat4x4_row<Type> mat4x4<Type>::get_row_4() const
    {
        return
        {
            data[to_utype(index::x4)], data[to_utype(index::y4)], data[to_utype(index::z4)], data[to_utype(index::w4)]
        };
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

    /*
    =================
    mat4x4::transpose
    =================
    */
    template<typename Type>
    inline mat4x4<Type> mat4x4<Type>::transpose(const mat4x4 &mat)
    {
        return
        {
            mat[to_utype(index::x1)], mat[to_utype(index::x2)], mat[to_utype(index::x3)], mat[to_utype(index::x4)],
            mat[to_utype(index::y1)], mat[to_utype(index::y2)], mat[to_utype(index::y3)], mat[to_utype(index::y4)],
            mat[to_utype(index::z1)], mat[to_utype(index::z2)], mat[to_utype(index::z3)], mat[to_utype(index::z4)],
            mat[to_utype(index::w1)], mat[to_utype(index::w2)], mat[to_utype(index::w3)], mat[to_utype(index::w4)]
        };
    }

}


#endif