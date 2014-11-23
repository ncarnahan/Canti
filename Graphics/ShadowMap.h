#ifndef GRAPHICS_SHADOW_MAP_H
#define GRAPHICS_SHADOW_MAP_H

#include <Math/Matrix4x4.h>
using namespace Math;
#include "Framebuffer.h"

namespace Graphics
{
    class ShadowMap
    {
    public:
        Framebuffer framebuffer;
        Matrix4x4 matrixPV;
        float bias;
        float strength;
    };
}

#endif
