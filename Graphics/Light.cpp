#include "Light.h"

namespace Graphics
{
    void Light::Directional(Quaternion rotation, Vector3 color, float intensity)
    {
        this->type = LightType::Directional;

        this->rotation = rotation;
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

    void Light::Spot(Vector3 position, Quaternion rotation, Vector3 color,
        float intensity, float radius, float angle, float innerPercent)
    {
        this->type = LightType::Spot;

        this->position = position;
        this->rotation = rotation;
        this->color = color;
        this->intensity = intensity;
        this->radius = radius;
        this->angle = angle;
        this->innerPercent = innerPercent;
    }
}
