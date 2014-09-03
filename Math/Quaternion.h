#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H

#include "Vector3.h"

namespace Math
{
    class Quaternion
    {
    public:
        float w, x, y, z;

        static const Quaternion identity;

    public:
        inline Quaternion() : w(1), x(0), y(0), z(0)
        {
        }

        inline Quaternion(float w, float x, float y, float z)
            : w(w), x(x), y(y), z(z)
        {
        }

        inline static Quaternion AngleAxis(float angle, Vector3 axis)
        {
            axis.Normalize();

            angle = angle / 2 * DEG2RAD;

            float cosAngle = std::cos(angle);
            float sinAngle = std::sin(angle);

            return Quaternion(cosAngle,
                axis.x * sinAngle,
                axis.y * sinAngle,
                axis.z * sinAngle);
        }
    };

    inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(
            lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
            lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x);
    }

    inline Vector3 operator*(const Quaternion& lhs, const Vector3& rhs)
    {
        //http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
        //t = 2 * cross(q.xyz, v)
        //v' = v + q.w * t + cross(q.xyz, t)
        Vector3 q = Vector3(lhs.x, lhs.y, lhs.z);
        Vector3 t = 2 * Vector3::Cross(q, rhs);
        return rhs + lhs.w * t + Vector3::Cross(q, t);
    }

    inline bool operator==(const Quaternion& lhs, const Quaternion& rhs)
    {
        return FloatEq(lhs.w, rhs.w) &&
            FloatEq(lhs.x, rhs.x) &&
            FloatEq(lhs.y, rhs.y) &&
            FloatEq(lhs.z, rhs.z);
    }

    inline bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
