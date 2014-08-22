#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

#include "Common.h"
#include <cassert>

namespace Math
{
    class Vector3;

    Vector3 operator+(Vector3 lhs, const Vector3& rhs);
    Vector3 operator-(Vector3 lhs, const Vector3& rhs);
    Vector3 operator*(Vector3 lhs, float rhs);
    Vector3 operator*(float lhs, Vector3 rhs);
    Vector3 operator/(Vector3 lhs, float rhs);

    class Vector3
    {
    public:
        float x, y, z;
    
        static const Vector3 zero;
        static const Vector3 right;
        static const Vector3 left;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 back;

    public:
        inline Vector3::Vector3() : x(0), y(0), z(0)
        {
        }

        inline explicit Vector3::Vector3(float s) : x(s), y(s), z(s)
        {
        }

        inline Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
        {
        }



        inline float operator[](const size_t i) const
        {
            assert(i < 3);
            return *(&x + i);
        }

        inline float& operator[](const size_t i)
        {
            assert(i < 3);
            return *(&x + i);
        }



        inline static float Dot(const Vector3& lhs, const Vector3& rhs)
        {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }

        inline static Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x);
        }



        inline float Vector3::Magnitude() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        inline float Vector3::MagnitudeSqr() const
        {
            return x * x + y * y + z * z;
        }

        inline void Vector3::Normalize()
        {
            *this /= Magnitude();
        }

        inline Vector3 Vector3::Normalized() const
        {
            return (*this) / Magnitude();
        }


    
        inline static float Distance(const Vector3& point1, const Vector3& point2)
        {
            return (point1 - point2).Magnitude();
        }

        inline static Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
        {
            return start + t * (end - start);
        }

        inline static Vector3 Slerp(const Vector3& start, const Vector3& end, float t)
        {
            float dot = Vector3::Dot(start, end);
            Clamp(dot, -1.0f, 1.0f);

            float theta = std::acos(dot) * t;
            Vector3 rel = end - start * dot;
            rel.Normalize();
            return (start * std::cos(theta)) + (rel * std::sin(theta));
        }

        inline static Vector3 Nlerp(const Vector3& start, const Vector3& end, float t)
        {
            return Lerp(start, end, t).Normalized();
        }



        inline Vector3 operator-() const
        {
            return Vector3(-x, -y, -z);
        }

        inline Vector3& operator+=(const Vector3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        inline Vector3& operator-=(const Vector3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        inline Vector3& operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        inline Vector3& operator/=(float rhs)
        {
            float scalar = 1 / rhs;

            (*this) *= scalar;

            return *this;
        }
    };

    inline Vector3 operator+(Vector3 lhs, const Vector3& rhs)
    {
        return lhs += rhs;
    }

    inline Vector3 operator-(Vector3 lhs, const Vector3& rhs)
    {
        return lhs -= rhs;
    }

    inline Vector3 operator*(Vector3 lhs, float rhs)
    {
        return lhs *= rhs;
    }

    inline Vector3 operator*(float lhs, Vector3 rhs)
    {
        return rhs *= lhs;
    }

    inline Vector3 operator/(Vector3 lhs, float rhs)
    {
        return lhs /= rhs;
    }

    inline bool operator==(const Vector3& lhs, const Vector3& rhs)
    {
        const float epsilon = 0.000001f;
        return std::abs(lhs.x - rhs.x) < epsilon &&
            std::abs(lhs.y - rhs.y) < epsilon &&
            std::abs(lhs.z - rhs.z) < epsilon;
    }

    inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
