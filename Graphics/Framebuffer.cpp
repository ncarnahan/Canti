#include "Framebuffer.h"
#include "Texture.h"

namespace Graphics
{
    Framebuffer::Framebuffer() :
        _fbo(0),
        _depthBuffer(0)
    {
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &_fbo);
    }

    void Framebuffer::Create(Texture* texture, int depthBits)
    {
        _width = texture->GetWidth();
        _height = texture->GetHeight();

        //Create and bind
        glGenFramebuffers(1, &_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        //Add optional depth buffer
        if (depthBits > 0)
        {
            GLenum depthComponent = GL_DEPTH_COMPONENT32;
            if (depthBits == 16) { depthComponent = GL_DEPTH_COMPONENT16; }
            else if (depthBits == 24) { depthComponent = GL_DEPTH_COMPONENT24; }

            glGenRenderbuffers(1, &_depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texture->GetWidth(), texture->GetHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
        }

        //Point framebuffer to texture
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->GetID(), 0);
        GLenum drawBuffers = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &drawBuffers);
    }

    void Framebuffer::Start()
    {
        //Save viewport
        glGetIntegerv(GL_VIEWPORT, _savedViewport);

        //Bind fbo
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glViewport(0, 0, _width, _height);
    }

    void Framebuffer::Stop()
    {
        //Clear framebuffer and restore default viewport
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(_savedViewport[0], _savedViewport[1], _savedViewport[2], _savedViewport[3]);
    }
}
