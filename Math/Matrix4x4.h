#ifndef MATH_MATRIX4X4_H
#define MATH_MATRIX4X4_H

#include "Vector3.h"
#include "Quaternion.h"

namespace Math
{
    class Matrix4x4;

    Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs);

    //Matrix4x4 is stored in column-major format.
    //
    //Here are the indices for the values array:
    // | 0   4   8   12 |
    // | 1   5   9   13 |
    // | 2   6   10  14 |
    // | 3   7   11  15 |
    //
    //Indices 12, 13, and 14 correspond to the translation component:
    // | 1   0   0   tx |
    // | 0   1   0   ty |
    // | 0   0   1   tz |
    // | 0   0   0   1  |
    //
    //Honestly, just ignore the internal storage mechanism. Use the RowCol and
    //ColRow methods to access it in the way makes the most sense to you.
    class Matrix4x4
    {
    private:
        float _values[16];

    public:
        static const Matrix4x4 zero;
        static const Matrix4x4 identity;

        //Constructors
        inline Matrix4x4()
        {
            _values[0] = 1;
            _values[1] = 0;
            _values[2] = 0;
            _values[3] = 0;
            
            _values[4] = 0;
            _values[5] = 1;
            _values[6] = 0;
            _values[7] = 0;
            
            _values[8] = 0;
            _values[9] = 0;
            _values[10] = 1;
            _values[11] = 0;
            
            _values[12] = 0;
            _values[13] = 0;
            _values[14] = 0;
            _values[15] = 1;
        }
        
        inline Matrix4x4(
            float r0c0, float r0c1, float r0c2, float r0c3,
            float r1c0, float r1c1, float r1c2, float r1c3,
            float r2c0, float r2c1, float r2c2, float r2c3,
            float r3c0, float r3c1, float r3c2, float r3c3)
        {
            _values[0] = r0c0;
            _values[1] = r1c0;
            _values[2] = r2c0;
            _values[3] = r3c0;
            
            _values[4] = r0c1;
            _values[5] = r1c1;
            _values[6] = r2c1;
            _values[7] = r3c1;
            
            _values[8] = r0c2;
            _values[9] = r1c2;
            _values[10] = r2c2;
            _values[11] = r3c2;
            
            _values[12] = r0c3;
            _values[13] = r1c3;
            _values[14] = r2c3;
            _values[15] = r3c3;
        }

        static Matrix4x4 FromPosition(Vector3 position);
        static Matrix4x4 FromRotation(Quaternion rotation);
        static Matrix4x4 FromScale(Vector3 scale);
        static Matrix4x4 FromTransform(Vector3 position, Quaternion rotation, Vector3 scale);
        static Matrix4x4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4x4 Perspective(float left, float right, float bottom, float top, float near, float far);
        static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float near, float far);

        //Accessors
        inline float operator[](const size_t i) const
        {
            assert(i < 16);
            return _values[i];
        }

        inline float& operator[](const size_t i)
        {
            assert(i < 16);
            return _values[i];
        }

        inline float& RowCol(size_t row, size_t col)
        {
            assert(row < 4 && col < 4);
            return _values[row + col * 4];
        }
        
        inline float& ColRow(size_t col, size_t row)
        {
            return RowCol(row, col);
        }
        
        //Operations
        Matrix4x4 GetInverse();
        Matrix4x4 GetTranspose();

        inline Matrix4x4& operator*=(const Matrix4x4& rhs)
        {
            // | 0   4   8   12 |
            // | 1   5   9   13 |
            // | 2   6   10  14 |
            // | 3   7   11  15 |

            auto& lhs = *this;
            
            lhs[0] = (lhs[0] * rhs[0]) + (lhs[4] * rhs[1]) + (lhs[8] * rhs[2]) + (lhs[12] * rhs[3]);
            lhs[1] = (lhs[1] * rhs[0]) + (lhs[5] * rhs[1]) + (lhs[9] * rhs[2]) + (lhs[13] * rhs[3]);
            lhs[2] = (lhs[2] * rhs[0]) + (lhs[6] * rhs[1]) + (lhs[10] * rhs[2]) + (lhs[14] * rhs[3]);
            lhs[3] = (lhs[3] * rhs[0]) + (lhs[7] * rhs[1]) + (lhs[11] * rhs[2]) + (lhs[15] * rhs[3]);

            lhs[4] = (lhs[0] * rhs[4]) + (lhs[4] * rhs[5]) + (lhs[8] * rhs[6]) + (lhs[12] * rhs[7]);
            lhs[5] = (lhs[1] * rhs[4]) + (lhs[5] * rhs[5]) + (lhs[9] * rhs[6]) + (lhs[13] * rhs[7]);
            lhs[6] = (lhs[2] * rhs[4]) + (lhs[6] * rhs[5]) + (lhs[10] * rhs[6]) + (lhs[14] * rhs[7]);
            lhs[7] = (lhs[3] * rhs[4]) + (lhs[7] * rhs[5]) + (lhs[11] * rhs[6]) + (lhs[15] * rhs[7]);
            
            lhs[8] = (lhs[0] * rhs[8]) + (lhs[4] * rhs[9]) + (lhs[8] * rhs[10]) + (lhs[12] * rhs[11]);
            lhs[9] = (lhs[1] * rhs[8]) + (lhs[5] * rhs[9]) + (lhs[9] * rhs[10]) + (lhs[13] * rhs[11]);
            lhs[10] = (lhs[2] * rhs[8]) + (lhs[6] * rhs[9]) + (lhs[10] * rhs[10]) + (lhs[14] * rhs[11]);
            lhs[11] = (lhs[3] * rhs[8]) + (lhs[7] * rhs[9]) + (lhs[11] * rhs[10]) + (lhs[15] * rhs[11]);
            
            lhs[12] = (lhs[0] * rhs[12]) + (lhs[4] * rhs[13]) + (lhs[8] * rhs[14]) + (lhs[12] * rhs[15]);
            lhs[13] = (lhs[1] * rhs[12]) + (lhs[5] * rhs[13]) + (lhs[9] * rhs[14]) + (lhs[13] * rhs[15]);
            lhs[14] = (lhs[2] * rhs[12]) + (lhs[6] * rhs[13]) + (lhs[10] * rhs[14]) + (lhs[14] * rhs[15]);
            lhs[15] = (lhs[3] * rhs[12]) + (lhs[7] * rhs[13]) + (lhs[11] * rhs[14]) + (lhs[15] * rhs[15]);

            return *this;
        }
    };

    inline bool operator==(const Matrix4x4& lhs, const Matrix4x4& rhs)
    {
        for (size_t i = 0; i < 16; i++)
        {
            if (!FloatEq(lhs[i], rhs[i]))
            {
                return false;
            }
        }
        return true;
    }

    inline bool operator!=(const Matrix4x4& lhs, const Matrix4x4& rhs)
    {
        return !(lhs == rhs);
    }

    inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
    {
        Matrix4x4 matrix;
            
        matrix[0] = (lhs[0] * rhs[0]) + (lhs[4] * rhs[1]) + (lhs[8] * rhs[2]) + (lhs[12] * rhs[3]);
        matrix[1] = (lhs[1] * rhs[0]) + (lhs[5] * rhs[1]) + (lhs[9] * rhs[2]) + (lhs[13] * rhs[3]);
        matrix[2] = (lhs[2] * rhs[0]) + (lhs[6] * rhs[1]) + (lhs[10] * rhs[2]) + (lhs[14] * rhs[3]);
        matrix[3] = (lhs[3] * rhs[0]) + (lhs[7] * rhs[1]) + (lhs[11] * rhs[2]) + (lhs[15] * rhs[3]);

        matrix[4] = (lhs[0] * rhs[4]) + (lhs[4] * rhs[5]) + (lhs[8] * rhs[6]) + (lhs[12] * rhs[7]);
        matrix[5] = (lhs[1] * rhs[4]) + (lhs[5] * rhs[5]) + (lhs[9] * rhs[6]) + (lhs[13] * rhs[7]);
        matrix[6] = (lhs[2] * rhs[4]) + (lhs[6] * rhs[5]) + (lhs[10] * rhs[6]) + (lhs[14] * rhs[7]);
        matrix[7] = (lhs[3] * rhs[4]) + (lhs[7] * rhs[5]) + (lhs[11] * rhs[6]) + (lhs[15] * rhs[7]);

        matrix[8] = (lhs[0] * rhs[8]) + (lhs[4] * rhs[9]) + (lhs[8] * rhs[10]) + (lhs[12] * rhs[11]);
        matrix[9] = (lhs[1] * rhs[8]) + (lhs[5] * rhs[9]) + (lhs[9] * rhs[10]) + (lhs[13] * rhs[11]);
        matrix[10] = (lhs[2] * rhs[8]) + (lhs[6] * rhs[9]) + (lhs[10] * rhs[10]) + (lhs[14] * rhs[11]);
        matrix[11] = (lhs[3] * rhs[8]) + (lhs[7] * rhs[9]) + (lhs[11] * rhs[10]) + (lhs[15] * rhs[11]);

        matrix[12] = (lhs[0] * rhs[12]) + (lhs[4] * rhs[13]) + (lhs[8] * rhs[14]) + (lhs[12] * rhs[15]);
        matrix[13] = (lhs[1] * rhs[12]) + (lhs[5] * rhs[13]) + (lhs[9] * rhs[14]) + (lhs[13] * rhs[15]);
        matrix[14] = (lhs[2] * rhs[12]) + (lhs[6] * rhs[13]) + (lhs[10] * rhs[14]) + (lhs[14] * rhs[15]);
        matrix[15] = (lhs[3] * rhs[12]) + (lhs[7] * rhs[13]) + (lhs[11] * rhs[14]) + (lhs[15] * rhs[15]);

        return matrix;
    }
};

#endif
