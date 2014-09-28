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
    private:
        LightType _type;

        Vector3 _position;      //  PS
        Vector3 _direction;     // D S
        Vector3 _color;         // DPS
        float _intensity;       // DPS
        float _radius;          //  PS
        float _angle;           //   S
        float _exponent;        //   S

    public:
        Light();
        ~Light();

        void Directional(Vector3 direction, Vector3 color, float intensity);
        void Point(Vector3 position, Vector3 color, float intensity, float radius);
        void Spot(Vector3 position, Vector3 direction, Vector3 color,
            float intensity, float radius, float angle, float exponent);

        inline void SetType(LightType type) { _type = type; }
        inline void SetPosition(Vector3 position) { _position = position; }
        inline void SetDirection(Vector3 direction) { _direction = direction; }
        inline void SetColor(Vector3 color) { _color = color; }
        inline void SetIntensity(float intensity) { _intensity = intensity; }
        inline void SetRadius(float radius) { _radius = radius; }
        inline void SetAngle(float angle) { _angle = angle; }
        inline void SetExponent(float exponent) { _exponent = exponent; }

        inline LightType GetType() { return _type; }
        inline Vector3 GetPosition() { return _position; }
        inline Vector3 GetDirection() { return _direction; }
        inline Vector3 GetColor() { return _color; }
        inline float GetIntensity() { return _intensity; }
        inline float GetRadius() { return _radius; }
        inline float GetAngle() { return _angle; }
        inline float GetExponent() { return _exponent; }
    };
}

#endif
