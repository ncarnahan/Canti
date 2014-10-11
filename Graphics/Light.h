#ifndef GRAPHICS_LIGHT_H
#define GRAPHICS_LIGHT_H

#include <Math/Vector3.h>
using namespace Math;

namespace Graphics
{
    enum class LightType
    {
        Directional,
        Point,
        Spot
    };

    class Light
    {
    public:
        LightType type;

        Vector3 position;      //  PS
        Vector3 direction;     // D S
        Vector3 color;         // DPS
        float intensity;       // DPS
        float radius;          //  PS
        float angle;           //   S
        float exponent;        //   S

        void Directional(Vector3 direction, Vector3 color, float intensity);
        void Point(Vector3 position, Vector3 color, float intensity, float radius);
        void Spot(Vector3 position, Vector3 direction, Vector3 color,
            float intensity, float radius, float angle, float exponent);
    };
}

#endif
