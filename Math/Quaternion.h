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
        //TODO
        return Vector3();
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
