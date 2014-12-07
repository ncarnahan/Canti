#include "Framebuffer.h"
#include "Texture.h"
#include <vector>

namespace Graphics
{
    Framebuffer::Framebuffer() :
        _fbo(0),
        _depthBuffer(0)
    {
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteRenderbuffers(1, &_depthBuffer);
        glDeleteFramebuffers(1, &_fbo);
    }

    bool Framebuffer::Create(uint32_t width, uint32_t height,
        RenderTarget* target, int count)
    {
        _width = width;
        _height = height;
        glGenFramebuffers(1, &_fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        std::vector<GLenum> drawBuffers;
        
        for (int i = 0; i < count; i++)
        {
            if (target[i].type == RenderTargetType::Renderbuffer)
            {
                //Only support depth renderbuffers
                if (target[i].attachment != RenderTargetAttachment::DepthAttachment)
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    return false;
                }

                glGenRenderbuffers(1, &_depthBuffer);
                glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                    GL_RENDERBUFFER, _depthBuffer);
            }
            else //Framebuffer
            {
                GLint attachment = (GLint)target[i].attachment;
                glFramebufferTexture(GL_FRAMEBUFFER,
                    attachment, target[i].texture->GetID(), 0);
                if (attachment != GL_DEPTH_ATTACHMENT)
                {
                    drawBuffers.push_back(attachment);
                }
            }
        }

        if (drawBuffers.size() > 0)
        {
            glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
        }
        else
        {
            glDrawBuffer(GL_NONE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    bool Framebuffer::CreateDepth(Texture& texture)
    {
        RenderTarget target;
        target.type = RenderTargetType::Framebuffer;
        target.attachment = RenderTargetAttachment::DepthAttachment;
        target.texture = &texture;
        return Create(texture.GetWidth(), texture.GetHeight(), &target, 1);
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
