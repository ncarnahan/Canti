#include "Texture.h"
#include "OpenGL.h"
#include <SDL_image.h>

namespace Graphics
{
    void FlipTextureVertically(int channels, int width, int height, void* data);

    Texture::Texture()
    {
        _textureId = 0;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_textureId);
    }

    bool Texture::Load(const char* fileName)
    {
        return Load(fileName, TextureLoadSettings());
    }

    bool Texture::Load(const char* fileName, TextureLoadSettings& settings)
    {
        auto surface = IMG_Load(fileName);
        
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        //TODO: Mipmaps
        //TODO: Make this stuff configurable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //Get the texture format
        GLenum format;
        GLenum internalFormat;
        switch (surface->format->BytesPerPixel) {
        case 4:
            format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
            if (settings.useSrgbColorSpace)
            {
                internalFormat = GL_SRGB8_ALPHA8;
            }
            else
            {
                internalFormat = GL_RGBA8;
            }
            break;
        case 3:
            format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
            if (settings.useSrgbColorSpace)
            {
                internalFormat = GL_SRGB8;
            }
            else
            {
                internalFormat = GL_RGB8;
            }
            break;
        case 1:
            //TODO: SDL_image doesn't appear to be loading 8-bit textures correctly
            //format = GL_RED;
            //internalFormat = GL_R8;
            //break;
        default:
            assert(false);
        }

        //Flip the texture so that the start is the lower left corner
        FlipTextureVertically(surface->format->BytesPerPixel,
            surface->w, surface->h, surface->pixels);

        //Upload to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0,
            format, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);

        return true;
    }

    void FlipTextureVertically(int bytesPerPixel, int width, int height, void* pixels)
    {
        char* data = (char*)pixels;
        int y1 = 0;
        int y2 = height - 1;

        int lineSize = bytesPerPixel * width;
        char* buffer = new char[lineSize];

        while (y1 < y2)
        {
            //Swap one line at a time
            memcpy(buffer, &data[y1 * lineSize], lineSize);
            memcpy(&data[y1 * lineSize], &data[y2 * lineSize], lineSize);
            memcpy(&data[y2 * lineSize], buffer, lineSize);

            //Next line
            y1++;
            y2--;
        }

        delete buffer;
    }



    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, _textureId);
    }
}
