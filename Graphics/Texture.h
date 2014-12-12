#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "OpenGL.h"

namespace Graphics
{
    enum class TextureFormat
    {
        RGB = GL_RGB8,
        RGBA = GL_RGBA8,
        sRGB = GL_SRGB8,
        sRGBA = GL_SRGB8_ALPHA8,
        Depth16 = GL_DEPTH_COMPONENT16,
        Depth24 = GL_DEPTH_COMPONENT24,
        Depth32 = GL_DEPTH_COMPONENT32,
        RGB16F = GL_RGB16F,
    };

    enum class TextureFilter
    {
        Nearest,
        Bilinear,
        Trilinear,
    };

    enum class TextureClamp
    {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP_TO_EDGE,
    };

    struct TextureLoadSettings
    {
        bool useSrgbColorSpace;
        bool generateMipmaps;
        TextureFilter filter;
        TextureClamp clamp;

        TextureLoadSettings() :
            useSrgbColorSpace(false),
            generateMipmaps(true),
            filter(TextureFilter::Trilinear),
            clamp(TextureClamp::Repeat)
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
        void Create(uint32_t width, uint32_t height,
            TextureFormat format, TextureLoadSettings& settings);
        void Bind();

        uint32_t GetWidth() { return _width; }
        uint32_t GetHeight() { return _height; }
        GLuint GetID() { return _textureId; }
    };
}

#endif
