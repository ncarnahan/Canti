#include "Light.h"

namespace Graphics
{
    void Light::Directional(Vector3 direction, Vector3 color, float intensity)
    {
        this->type = LightType::Directional;

        this->direction = direction;
        this->color = color;
        this->intensity = intensity;
    }

    void Light::Point(Vector3 position, Vector3 color, float intensity, float radius)
    {
        this->type = LightType::Point;

        this->position = position;
        this->color = color;
        this->intensity = intensity;
        this->radius = radius;
    }

    void Light::Spot(Vector3 position, Vector3 direction, Vector3 color,
        float intensity, float radius, float angle, float innerPercent)
    {
        this->type = LightType::Spot;

        this->position = position;
        this->direction = direction;
        this->color = color;
        this->intensity = intensity;
        this->radius = radius;
        this->angle = angle;
        this->innerPercent = innerPercent;
    }
}
