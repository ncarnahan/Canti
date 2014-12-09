#ifndef APPLICATION_GBUFFER_H
#define APPLICATION_GBUFFER_H

#include <Graphics/Texture.h>
#include <Graphics/Framebuffer.h>
using namespace Graphics;

class GBuffer
{
public:
    Texture depthTexture;
    Texture colorTexture;
    Texture normalTexture;
    Framebuffer framebuffer;

    void Create(int width, int height);
};

#endif
