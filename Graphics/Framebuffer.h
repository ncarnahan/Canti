#ifndef GRAPHICS_FRAMEBUFFER_H
#define GRAPHICS_FRAMEBUFFER_H

#include "OpenGL.h"

namespace Graphics
{
    class Texture;

    enum class RenderTargetType
    {
        Renderbuffer,
        Framebuffer,
    };

    enum class RenderTargetAttachment
    {
        DepthAttachment = GL_DEPTH_ATTACHMENT,
        ColorAttachment0 = GL_COLOR_ATTACHMENT0,
        ColorAttachment1 = GL_COLOR_ATTACHMENT1,
        ColorAttachment2 = GL_COLOR_ATTACHMENT2,
        ColorAttachment3 = GL_COLOR_ATTACHMENT3,
    };

    struct RenderTarget
    {
        RenderTargetType type;
        RenderTargetAttachment attachment;
        Texture* texture;   //Valid if type == Framebuffer
    };

    class Framebuffer
    {
    private:
        GLuint _fbo;
        GLuint _depthBuffer;
        uint32_t _width, _height;
        GLint _savedViewport[4];

    public:
        Framebuffer();
        ~Framebuffer();

        bool Framebuffer::Create(uint32_t width, uint32_t height,
            RenderTarget* target, int count);
        bool CreateDepth(Texture& texture);
        void Start();
        void Stop();
    };
}

#endif
