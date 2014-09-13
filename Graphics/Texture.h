#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "OpenGL.h"

namespace Graphics
{
    struct TextureLoadSettings
    {
        bool useSrgbColorSpace;

        TextureLoadSettings() :
            useSrgbColorSpace(false)
        {
        }
    };

    class Texture
    {
    private:
        GLuint _textureId;

    public:
        Texture();
        ~Texture();

        bool Load(const char* fileName);
        bool Load(const char* fileName, TextureLoadSettings& settings);
        void Bind();
    };
}

#endif
