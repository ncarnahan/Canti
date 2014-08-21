#include "Texture.h"
#include "OpenGL.h"
#include <SDL_image.h>

namespace Graphics
{
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
            internalFormat = GL_RGBA8;
            break;
        case 3:
            format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
            internalFormat = GL_RGB8;
            break;
        case 1:
            //TODO: SDL_image doesn't appear to be loading 8-bit textures correctly
            //format = GL_RED;
            //internalFormat = GL_R8;
            //break;
        default:
            assert(false);
        }

        //Upload to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0,
            format, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);

        return true;
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, _textureId);
    }
}
