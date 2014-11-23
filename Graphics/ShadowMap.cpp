#include "ShadowMap.h"
#include "Light.h"

namespace Graphics
{
    void ShadowMap::Init(Light& light)
    {
        switch (light.type)
        {
        case LightType::Directional:
            //TODO: 
            projMatrix = Matrix4x4::Orthographic(-15, 15, -15, 15, 0, 100);
            break;
        case LightType::Point:
            projMatrix = Matrix4x4::Perspective(90, 1, 0.01f, light.radius);
            break;
        case LightType::Spot:
            projMatrix = Matrix4x4::Perspective(light.angle, 1, 0.01f, light.radius);
            break;
        default:
            break;
        }
    }
}
