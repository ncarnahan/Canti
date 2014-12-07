#ifndef GRAPHICS_SHADOW_MAP_H
#define GRAPHICS_SHADOW_MAP_H

#include <Math/Matrix4x4.h>
using namespace Math;
#include "Framebuffer.h"

namespace Graphics
{
    class Light;

    class ShadowMap
    {
    public:
        Framebuffer framebuffer;
        Texture* texture;
        Matrix4x4 projMatrix;
        Matrix4x4 viewMatrix;
        float bias;
        float strength;

        ShadowMap() : strength(1)
        {
        }
        
        void Init(Light& light, Texture& texture);
    };
}

#endif
