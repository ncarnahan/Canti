#include "GBuffer.h"

void GBuffer::Create(int width, int height)
{
    TextureLoadSettings settings;
    settings.clamp = TextureClamp::Clamp;
    settings.filter = TextureFilter::Nearest;

    depthTexture.Create(width, height, TextureFormat::Depth32, settings);
    colorTexture.Create(width, height, TextureFormat::sRGB, settings);
    specularTexture.Create(width, height, TextureFormat::sRGBA, settings);
    normalTexture.Create(width, height, TextureFormat::RGB16F, settings);


    RenderTarget targets[4];
    targets[0].type = RenderTargetType::Framebuffer;
    targets[0].attachment = RenderTargetAttachment::DepthAttachment;
    targets[0].texture = &depthTexture;

    targets[1].type = RenderTargetType::Framebuffer;
    targets[1].attachment = RenderTargetAttachment::ColorAttachment0;
    targets[1].texture = &colorTexture;

    targets[2].type = RenderTargetType::Framebuffer;
    targets[2].attachment = RenderTargetAttachment::ColorAttachment1;
    targets[2].texture = &specularTexture;

    targets[3].type = RenderTargetType::Framebuffer;
    targets[3].attachment = RenderTargetAttachment::ColorAttachment2;
    targets[3].texture = &normalTexture;

    framebuffer.Create(width, height, targets, 4);
}
