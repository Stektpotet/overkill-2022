#pragma once

#include <overkill/graphics_internal/Texture.hpp>

#include <unordered_map>
#include <vector>

namespace OK
{
    using FrameBufferRenderTexture = Texture2D;
    class FrameBufferRenderBuffer
    {
    private:
        GLuint m_id;
        TextureSamplingOptions rTexParams;
    public:
        FrameBufferRenderBuffer();
        inline const GLuint getID() const;
        inline explicit operator GLuint() const;
        void clean();
        void bind() const;
        void unbind() const;
    };

    class FrameBuffer //REF: https://www.youtube.com/watch?v=21UsMuFTN0k
    {
    private:
        GLuint m_id;
        std::string m_tag;

        uint16_t m_width, m_height;
        TextureSamplingOptions m_rTexParams;

        std::unordered_map<std::string, FrameBufferRenderTexture> m_renderTextures;
        std::unordered_map<std::string, FrameBufferRenderBuffer>  m_renderBuffers;

    public:
        FrameBuffer(
            const uint16_t width,
            const uint16_t height,
            const TextureSamplingOptions rTexParams = TextureSamplingOptions()
            //,
            //const std::unordered_map<C::Tag, FrameBufferRenderTexture> texAttachments,
            //const std::unordered_map<C::Tag, FrameBufferRenderBuffer> bufAttachments
        );

        auto attachRenderTexture(const int colorAttachmentNr = 0)->FrameBufferRenderTexture;
        auto attachDepthTexture(const GLenum depthComponent = GL_DEPTH_COMPONENT32)->FrameBufferRenderTexture;

        FrameBufferRenderBuffer attachDepthBuffer(const uint16_t width, const uint16_t height);

        FrameBufferRenderTexture getAttachedRenderTexture(const std::string& tag);
        FrameBufferRenderBuffer getAttachedRenderBuffer(const std::string& tag);

        void bind() const;
        void unbind() const;

        void clean();

    };

}