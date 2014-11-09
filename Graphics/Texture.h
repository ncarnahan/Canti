#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "OpenGL.h"

namespace Graphics
{
    enum class TextureFilter
    {
        Nearest,
        Trilinear,
    };

    struct TextureLoadSettings
    {
        bool useSrgbColorSpace;
        bool generateMipmaps;
        TextureFilter filter;

        TextureLoadSettings() :
            useSrgbColorSpace(false),
            generateMipmaps(true),
            filter(TextureFilter::Trilinear)
        {
        }

        int GetMinFilter();
        int GetMagFilter();
    };

    class Texture
    {
    private:
        GLuint _textureId;
        uint32_t _width, _height;

    public:
        Texture();
        ~Texture();

        bool Load(const char* fileName);
        bool Load(const char* fileName, TextureLoadSettings& settings);
        void Bind();

        uint32_t GetWidth() { return _width; }
        uint32_t GetHeight() { return _height; }
    };
}

#endif
