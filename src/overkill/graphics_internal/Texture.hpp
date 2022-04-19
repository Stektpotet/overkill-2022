#pragma once
#include <vector>
#include <glad/glad.h>
#include <stdint.h>

namespace OK
{
    //TODO: Cleanup, don't inline everything
    // TODO: Move RawTexture
    template<typename TPixel = std::uint8_t>
    struct RawTexture
    {
        std::uint16_t width = 0u;
        std::uint16_t height = 0u;
        std::uint16_t channels = 4u;
        TPixel* pixels;

        RawTexture(std::uint16_t width, std::uint16_t height, std::uint16_t channels, TPixel* data = nullptr)
            : width{ width }, height{ height }, channels{ channels }
        {
            size_t data_size = size_t(width * height * channels) * sizeof(TPixel);
            pixels = (TPixel*)malloc(data_size);
            if (data) {
                memcpy(pixels, data, data_size);
            }
        }
        void clear()
        {
            free(pixels);
            pixels = nullptr;
        }
    };

    using RawTexture8 = RawTexture<std::uint8_t>;
    using RawTexture16 = RawTexture<std::uint16_t>;

    struct TextureSamplingOptions
    {
        GLenum
            minFilter = GL_NEAREST_MIPMAP_LINEAR,
            maxFilter = GL_LINEAR;
        GLenum
            sWrap = GL_REPEAT,
            tWrap = GL_REPEAT,
            rWrap = GL_REPEAT;

        TextureSamplingOptions() = default;
        TextureSamplingOptions(
            GLenum filterMode, GLenum wrapMode
        ) : minFilter{ filterMode },
            maxFilter{ filterMode },
            sWrap{ wrapMode },
            tWrap{ wrapMode },
            rWrap{ wrapMode }
        {}

        TextureSamplingOptions(
            GLenum minFilterMode,
            GLenum maxFilterMode,
            GLenum sWrapMode,
            GLenum tWrapMode,
            GLenum rWrapMode
        ) : minFilter{ minFilterMode },
            maxFilter{ maxFilterMode },
            sWrap{ sWrapMode },
            tWrap{ tWrapMode },
            rWrap{ rWrapMode }
        {}
    };

    class Texture3D
    {
    protected:
        GLuint id;
        uint16_t width, height, depth, channels;
        TextureSamplingOptions samplingOptions;
    public:
        Texture3D() = default;
        Texture3D(uint16_t width, uint16_t height, uint16_t depth, uint16_t channels, TextureSamplingOptions options = TextureSamplingOptions());
        // DO NOT PUT THIS IN A DESTRUCTOR, we don't want to clean everytime a texture falls out of scope, the texture may be intended to remain on GPU
        inline void clean() { glDeleteTextures(1, &id); }

        inline const bool valid() const { return id != 0; }
        inline const GLuint getID() const { return id; }

        inline const uint16_t  getWidth() const     { return width; }
        inline const uint16_t  getHeight() const    { return height; }
        inline const uint16_t  getDepth() const     { return depth; }
        inline const uint16_t  getChannels() const  { return channels; }

        void bind(GLuint slot = 0) const;

        void unbind() const;
        void setSamplingOptions(TextureSamplingOptions options);
    };

    class TextureBase 

    {
    protected:
        GLuint id;
        uint16_t width, height, channels;
        TextureSamplingOptions samplingOptions;
    public:
        TextureBase() = default;
        TextureBase(uint16_t width, uint16_t height, uint16_t channels, TextureSamplingOptions options = TextureSamplingOptions());

        // DO NOT PUT THIS IN A DESTRUCTOR, we don't want to clean everytime a texture falls out of scope, the texture may be intended to remain on GPU
        inline void clean() { glDeleteTextures(1, &id); }

        inline const bool valid() const { return id != 0; }
        inline const GLuint getID() const { return id; }

        inline const uint16_t const getWidth() { return width; }
        inline const uint16_t const getHeight() { return height; }
        inline const uint16_t const getChannels() { return channels; }

        // TODO: Do we need these to be virtual?
        virtual void bind(GLuint slot = 0) const = 0; 

        virtual void unbind() const = 0;
        virtual void setSamplingOptions(TextureSamplingOptions options) const = 0;
        virtual RawTexture8 fetchRaw() const = 0;
    };

    template <GLenum TextureType>
    class Texture : public TextureBase
    {
    protected:
    public:
        Texture() = default; //TODO: can we maybe put the sampling options into base?
        Texture(uint16_t width, uint16_t height, uint16_t channels, TextureSamplingOptions options = TextureSamplingOptions())
            : TextureBase(width, height, channels, options) {
            glGenTextures(1, &id);
            glBindTexture(TextureType, id);

            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, options.minFilter);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, options.maxFilter);

            glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, options.sWrap);
            glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, options.tWrap);
        }

        /// <summary>
        /// Create a texture object filled with the data passed
        /// </summary>
        Texture(RawTexture8& raw, TextureSamplingOptions options = TextureSamplingOptions())
            : TextureBase(raw.width, raw.height, raw.channels, options)
        {
            glGenTextures(1, &id);
            glBindTexture(TextureType, id);

            GLenum raw_format = GL_RGBA;
            switch (raw.channels) // TODO: Add support for differing bit-depth (currently 8bit depth is permitted)
            {
            case 1: raw_format = GL_RED; break;
            case 2: raw_format = GL_RG;  break;
            case 3: raw_format = GL_RGB;  break;
            }

            glTexImage2D(TextureType, 0, GL_RGBA, width,  height, 0, raw_format, GL_UNSIGNED_BYTE, raw.pixels);
            glGenerateMipmap(TextureType);
            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, options.minFilter);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, options.maxFilter);

            glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, options.sWrap);
            glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, options.tWrap);
        }


        inline void bind(GLuint slot = 0) const override {
            glBindTextureUnit(slot, id);
        }
        inline void unbind() const override {
            glBindTexture(TextureType, 0);
        }

        inline void setSamplingOptions(TextureSamplingOptions options) const override
        {
            glBindTexture(TextureType, id);

            glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, options.minFilter);
            glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, options.maxFilter);

            glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, options.sWrap);
            glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, options.tWrap);

            //NOTE: Might be worth restoring previous bound texture...
        }


        /// <summary>
        /// fetch the uploaded texture from the GPU-memory as raw pixels.
        /// </summary>
        inline RawTexture8 fetchRaw() const override
        {
            RawTexture8 raw = RawTexture8(width, height, 4u);
            glGetTextureImage(TextureType, 0, GL_RGBA, GL_UNSIGNED_BYTE, width * height, &(raw.pixels)[0]);
            return raw;
        }
    };

    using Texture2D = Texture<GL_TEXTURE_2D>; //declare the type as an implementation

    class Texture2DArray final : public Texture<GL_TEXTURE_2D_ARRAY>
    {
    private:
        uint16_t depth/*, sliceDimension*/;
    public:
        Texture2DArray(RawTexture8& raw, TextureSamplingOptions options = TextureSamplingOptions());
        inline uint16_t getDepth() { return depth; }

        inline RawTexture8 fetchRaw(int elementX, int elementY) const
        {
            RawTexture8 raw = RawTexture8(width, height, 4u);
            glGetTextureImage(id, 0, GL_RGBA, GL_UNSIGNED_BYTE, width * height, &(raw.pixels)[0]);
            return raw;
        }
    };
}
