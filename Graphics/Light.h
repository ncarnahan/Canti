#ifndef GRAPHICS_LIGHT_H
#define GRAPHICS_LIGHT_H

#include <Math/Vector3.h>
#include <Math/Quaternion.h>
using namespace Math;

namespace Graphics
{
    enum class LightType
    {
        Directional,
        Point,
        Spot,
        Ambient,    //Only used for deferred shading
    };

    class ShadowMap;

    class Light
    {
    public:
        LightType type;

        Vector3 position;      //  PS
        Quaternion rotation;   // D S
        Vector3 color;         // DPS
        float intensity;       // DPS
        float radius;          //  PS
        float angle;           //   S
        float innerPercent;    //   S

        ShadowMap* shadowMap;

        void Directional(Quaternion rotation, Vector3 color, float intensity);
        void Point(Vector3 position, Vector3 color, float intensity, float radius);
        void Spot(Vector3 position, Quaternion rotation, Vector3 color,
            float intensity, float radius, float angle, float innerPercent);
        void Ambient(Vector3 color);
    };
}

#endif
