#include "FrameBuffer.hpp"
#include <utilities/window.hpp> // TODO: this isnt so nice to be including here...

namespace OK
{
    FrameBuffer::FrameBuffer(
        const uint16_t width,
        const uint16_t height,
        const TextureSamplingOptions rTexParams /*= TextureParams()*/
    ) : m_width{ width },
        m_height{ height },
        m_rTexParams{ rTexParams }
    {
        glGenFramebuffers(1, &m_id);
        bind();
        /*
            std::vector<GLuint> attachmentIDs;
            attachmentIDs.reserve(texAttachments.size() + bufAttachments.size());
            for (const auto&[tag, tex] : texAttachments)
            {
                attachmentIDs.push_back(GLuint(tex));
            }
            for (const auto&[tag, buf] : texAttachments)
            {
                attachmentIDs.push_back(GLuint(buf));
            }
            GLCall(glDrawBuffers(attachmentIDs.size(), attachmentIDs.data()));*/
    }

    auto FrameBuffer::attachRenderTexture(const int colorAttachmentNr /*= 0*/)->FrameBufferRenderTexture
    {
        //ASSERT(colorAttachmentNr < 16);
        FrameBufferRenderTexture renderTexture = FrameBufferRenderTexture(m_width, m_height, 3, m_rTexParams);
        //implicitly bound on creation, no need to bind! :-)

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); //allocate texture space on buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentNr, GL_TEXTURE_2D, renderTexture.getID(), 0);

        return renderTexture;
    }

    auto FrameBuffer::attachDepthTexture(GLenum depthComponent /*= GL_DEPTH_COMPONENT32*/)->FrameBufferRenderTexture
    {
        FrameBufferRenderTexture renderTexture = FrameBufferRenderTexture(m_width, m_height, 1, m_rTexParams);

        glTexImage2D(GL_TEXTURE_2D, 0, depthComponent, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); //allocate texture space on buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTexture.getID(), 0);

        return renderTexture;
    }

    //WARNING: this is dependent on execution immediately after createDepthTextureAttachment
    FrameBufferRenderBuffer FrameBuffer::attachDepthBuffer(const uint16_t width, const uint16_t height)
    {
        FrameBufferRenderBuffer depthBuffer = FrameBufferRenderBuffer();

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.getID());
        return depthBuffer;
    }

    void FrameBuffer::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);//TODO what is this - or rather why?
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glViewport(0, 0, m_width, m_height); //set the viewport to use this rendertextures sizes.
    }

    void FrameBuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // TODO: have these be specified through arguments instead
    }

    void FrameBuffer::clean()
    {
        glDeleteFramebuffers(1, &m_id);
        for (auto&[tag, tex] : m_renderTextures)
        {
            tex.clean();
        }
        for (auto&[tag, buf] : m_renderBuffers)
        {
            buf.clean();
        }
    }


    FrameBufferRenderTexture FrameBuffer::getAttachedRenderTexture(const std::string& tag)
    {
        return m_renderTextures.at(tag);
    }

    FrameBufferRenderBuffer FrameBuffer::getAttachedRenderBuffer(const std::string& tag)
    {
        return m_renderBuffers.at(tag);
    }

    FrameBufferRenderBuffer::FrameBufferRenderBuffer()
    {
        glGenRenderbuffers(1, &m_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    inline const GLuint FrameBufferRenderBuffer::getID() const { return m_id; }

    inline FrameBufferRenderBuffer::operator GLuint() const { return m_id; }

    void FrameBufferRenderBuffer::clean()
    {
        glDeleteRenderbuffers(1, &m_id);
    }

    void FrameBufferRenderBuffer::bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    void FrameBufferRenderBuffer::unbind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

}
