#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "OpenGL.h"

namespace Graphics
{
    class Texture
    {
    private:
        GLuint _textureId;

    public:
        Texture();
        ~Texture();

        bool Load(const char* fileName);
        void Bind();
    };
}

#endif
