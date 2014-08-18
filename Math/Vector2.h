#ifndef MATH_VECTOR2_H
#define MATH_VECTOR2_H

#include "Common.h"
#include <cassert>

namespace Math
{
    class Vector2;

    Vector2 operator+(Vector2 lhs, const Vector2& rhs);
    Vector2 operator-(Vector2 lhs, const Vector2& rhs);
    Vector2 operator*(Vector2 lhs, float rhs);
    Vector2 operator*(float lhs, Vector2 rhs);
    Vector2 operator/(Vector2 lhs, float rhs);

    class Vector2
    {
    public:
        float x, y;
    
        static const Vector2 zero;
        static const Vector2 right;
        static const Vector2 left;
        static const Vector2 up;
        static const Vector2 down;

    public:
        inline Vector2::Vector2() : x(0), y(0)
        {
        }

        inline explicit Vector2::Vector2(float s) : x(s), y(s)
        {
        }

        inline Vector2::Vector2(float x, float y) : x(x), y(y)
        {
        }



        inline float operator[](const size_t i) const
	    {
		    assert(i < 2);
		    return *(&x + i);
	    }

	    inline float& operator[](const size_t i)
	    {
		    assert(i < 2);
		    return *(&x + i);
	    }



        inline static float Dot(const Vector2& lhs, const Vector2& rhs)
        {
            return lhs.x * rhs.x + lhs.y * rhs.y;
        }



        inline float Vector2::Magnitude() const
        {
            return std::sqrt(x * x + y * y);
        }

        inline float Vector2::MagnitudeSqr() const
        {
            return x * x + y * y;
        }

        inline void Vector2::Normalize()
        {
            *this /= Magnitude();
        }

        inline Vector2 Vector2::Normalized() const
        {
            return (*this) / Magnitude();
        }


    
        inline static float Distance(const Vector2& point1, const Vector2& point2)
        {
            return (point1 - point2).Magnitude();
        }

        inline static Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
        {
            return start + t * (end - start);
        }

        inline static Vector2 Slerp(const Vector2& start, const Vector2& end, float t)
        {
            float dot = Vector2::Dot(start, end);
            Clamp(dot, -1.0f, 1.0f);

            float theta = std::acos(dot) * t;
            Vector2 rel = end - start * dot;
            rel.Normalize();
            return (start * std::cos(theta)) + (rel * std::sin(theta));
        }

        inline static Vector2 Nlerp(const Vector2& start, const Vector2& end, float t)
        {
            return Lerp(start, end, t).Normalized();
        }



        inline Vector2 operator-() const
        {
            return Vector2(-x, -y);
        }

        inline Vector2& operator+=(const Vector2& rhs)
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        inline Vector2& operator-=(const Vector2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        inline Vector2& operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;

            return *this;
        }

        inline Vector2& operator/=(float rhs)
        {
            float scalar = 1 / rhs;

            (*this) *= scalar;

            return *this;
        }
    };

    inline Vector2 operator+(Vector2 lhs, const Vector2& rhs)
    {
        return lhs += rhs;
    }

    inline Vector2 operator-(Vector2 lhs, const Vector2& rhs)
    {
        return lhs -= rhs;
    }

    inline Vector2 operator*(Vector2 lhs, float rhs)
    {
        return lhs *= rhs;
    }

    inline Vector2 operator*(float lhs, Vector2 rhs)
    {
        return rhs *= lhs;
    }

    inline Vector2 operator/(Vector2 lhs, float rhs)
    {
        return lhs /= rhs;
    }

    inline bool operator==(const Vector2& lhs, const Vector2& rhs)
    {
        const float epsilon = 0.000001f;
        return std::abs(lhs.x - rhs.x) < epsilon &&
            std::abs(lhs.y - rhs.y) < epsilon;
    }

    inline bool operator!=(const Vector2& lhs, const Vector2& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
