#include "Light.h"

namespace Graphics
{
    Light::Light()
    {
    }

    Light::~Light()
    {
    }

    void Light::Directional(Vector3 direction, Vector3 color, float intensity)
    {
        _type = LightType::Directional;

        _direction = direction;
        _color = color;
        _intensity = intensity;
    }

    void Light::Point(Vector3 position, Vector3 color, float intensity, float radius)
    {
        _type = LightType::Point;

        _position = position;
        _color = color;
        _intensity = intensity;
        _radius = radius;
    }

    void Light::Spot(Vector3 position, Vector3 direction, Vector3 color,
        float intensity, float radius, float angle, float exponent)
    {
        _type = LightType::Spot;

        _position = position;
        _direction = direction;
        _color = color;
        _intensity = intensity;
        _radius = radius;
        _angle = angle;
        _exponent = exponent;
    }
}
