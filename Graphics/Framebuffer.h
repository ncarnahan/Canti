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

    public:
        Framebuffer();
        ~Framebuffer();

        void Create(Texture* texture, int depthBits);
        void Start();
        void Stop();
    };
}

#endif
