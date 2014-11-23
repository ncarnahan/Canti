#ifndef GRAPHICS_FRAMEBUFFER_H
#define GRAPHICS_FRAMEBUFFER_H

#include "OpenGL.h"

namespace Graphics
{
    class Texture;

    class Framebuffer
    {
    private:
        GLuint _fbo;
        GLuint _depthBuffer;
        uint32_t _width, _height;
        GLint _savedViewport[4];
        Texture* _texture;

    public:
        Framebuffer();
        ~Framebuffer();

        bool Create(Texture& texture, int depthBits);
        bool CreateDepth(Texture& texture);
        void Start();
        void Stop();

        Texture* GetTexture() { return _texture; }
    };
}

#endif
