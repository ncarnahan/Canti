#ifndef MATH_COMMON_H
#define MATH_COMMON_H

#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>

namespace Math
{
    const float PI = (float)3.14159265358979323846;
    const float DEG2RAD = 2 * PI / 360;
    const float RAD2DEG = 360 / (2 * PI);

    inline int Min(int value1, int value2)
    {
        return (value1 < value2) ? value1 : value2;
    }

    inline int Max(int value1, int value2)
    {
        return (value1 > value2) ? value1 : value2;
    }

    inline int Clamp(int value, int low, int high)
    {
        assert(high > low);
        return Min(Max(value, low), high);
    }

    inline float Min(float value1, float value2)
    {
        return (value1 < value2) ? value1 : value2;
    }

    inline float Max(float value1, float value2)
    {
        return (value1 > value2) ? value1 : value2;
    }

    inline float Clamp(float value, float low, float high)
    {
        assert(high > low);
        return Min(Max(value, low), high);
    }
}

#endif