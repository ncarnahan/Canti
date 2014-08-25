#include "Matrix4x4.h"

namespace Math
{
    const Matrix4x4 Matrix4x4::zero = Matrix4x4(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);

    const Matrix4x4 Matrix4x4::identity = Matrix4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

    Matrix4x4 Matrix4x4::FromPosition(Vector3 position)
    {
        Matrix4x4 matrix = Matrix4x4::identity;
        
        //Translation matrix has values in the last column
        // | 1  0  0  x |
        // | 0  1  0  y |
        // | 0  0  1  z |
        // | 0  0  0  1 |
        matrix.RowCol(0, 3) = position.x;
        matrix.RowCol(1, 3) = position.y;
        matrix.RowCol(2, 3) = position.z;
        
        return matrix;
    }
    
    Matrix4x4 Matrix4x4::FromRotation(Quaternion rotation)
    {
        //rotation.Normalize();
        
        Matrix4x4 matrix = Matrix4x4::identity;
        
        float a = rotation.w;
        float b = rotation.x;
        float c = rotation.y;
        float d = rotation.z;
        
        float aSqr = a * a;
        float bSqr = b * b;
        float cSqr = c * c;
        float dSqr = d * d;
        
        matrix.RowCol(0, 0) = aSqr + bSqr - cSqr - dSqr;
        matrix.RowCol(0, 1) = (2 * b * c) - (2 * a * d);
        matrix.RowCol(0, 2) = (2 * b * d) + (2 * a * c);
        
        matrix.RowCol(1, 0) = (2 * b * c) + (2 * a * d);
        matrix.RowCol(1, 1) = aSqr - bSqr + cSqr - dSqr;
        matrix.RowCol(1, 2) = (2 * c * d) - (2 * a * b);
        
        matrix.RowCol(2, 0) = (2 * b * d) - (2 * a * c);
        matrix.RowCol(2, 1) = (2 * c * d) + (2 * a * b);
        matrix.RowCol(2, 2) = aSqr - bSqr - cSqr + dSqr;
        
        return matrix;
    }
    
    Matrix4x4 Matrix4x4::FromScale(Vector3 scale)
    {
        Matrix4x4 matrix = Matrix4x4::identity;
        
        //Scalar matrix has values on the diagonal
        // | x  0  0  0 |
        // | 0  y  0  0 |
        // | 0  0  z  0 |
        // | 0  0  0  1 |
        matrix.RowCol(0, 0) = scale.x;
        matrix.RowCol(1, 1) = scale.y;
        matrix.RowCol(2, 2) = scale.z;
        
        return matrix;
    }

    Matrix4x4 Matrix4x4::FromTransform(Vector3 position, Quaternion rotation, Vector3 scale)
    {
        return FromPosition(position) * FromRotation(rotation) * FromScale(scale);
    }

    Matrix4x4 Matrix4x4::Perspective(float verticalFov, float aspect, float near, float far)
    {
        float width;
        float height;
        
        height = Math::TanDeg(verticalFov / 2) * near;
        width = height * aspect;
        
        return Perspective(-width, width, -height, height, near, far);
    }
    
    Matrix4x4 Matrix4x4::Perspective(float left, float right, float bottom, float top, float near, float far)
    {
        //http://www.songho.ca/opengl/gl_projectionmatrix.html
        Matrix4x4 matrix = Matrix4x4::zero;
        
        matrix.RowCol(0, 0) = 2 * near / (right - left);
        matrix.RowCol(0, 2) = (right + left) / (right - left);
        
        matrix.RowCol(1, 1) = 2 * near / (top - bottom);
        matrix.RowCol(1, 2) = (top + bottom) / (top - bottom);
        
        matrix.RowCol(2, 2) = -(far + near) / (far - near);
        matrix.RowCol(2, 3) = -2 * far * near / (far - near);
        
        matrix.RowCol(3, 2) = -1;
        
        return matrix;
    }

    Matrix4x4 Matrix4x4::Orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        //http://www.songho.ca/opengl/gl_projectionmatrix.html
        Matrix4x4 matrix = Matrix4x4::identity;
        
        //Scale components
        matrix.RowCol(0, 0) = 2 / (right - left);
        matrix.RowCol(1, 1) = 2 / (top - bottom);
        matrix.RowCol(2, 2) = -2 / (far - near);
        
        //Translate components
        matrix.RowCol(0, 3) = -(right + left) / (right - left);
        matrix.RowCol(1, 3) = -(top + bottom) / (top - bottom);
        matrix.RowCol(2, 3) = -(far + near) / (far - near);
        
        return matrix;
    }
    
    Matrix4x4 Matrix4x4::GetInverse()
    {
        //TODO
        return Matrix4x4::zero;
    }
    
    Matrix4x4 Matrix4x4::GetTranspose()
    {
        return Matrix4x4(
            _values[0], _values[1], _values[2], _values[3],
            _values[4], _values[5], _values[6], _values[7],
            _values[8], _values[9], _values[10], _values[11],
            _values[12], _values[13], _values[14], _values[15]);
    }
}
