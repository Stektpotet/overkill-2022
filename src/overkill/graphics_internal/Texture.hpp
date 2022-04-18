#pragma once
#include <vector>
#include <glad/glad.h>
#include <stdint.h>


inline const char* GLEnumToString(GLenum e)
{
#define GFX_TO_STRING_GENERATOR(x) case x: return #x; break;
    switch (e)
    {
        // shader:
        GFX_TO_STRING_GENERATOR(GL_VERTEX_SHADER);
        GFX_TO_STRING_GENERATOR(GL_GEOMETRY_SHADER);
        GFX_TO_STRING_GENERATOR(GL_FRAGMENT_SHADER);

        // buffer usage:
        GFX_TO_STRING_GENERATOR(GL_STREAM_DRAW);
        GFX_TO_STRING_GENERATOR(GL_STREAM_READ);
        GFX_TO_STRING_GENERATOR(GL_STREAM_COPY);
        GFX_TO_STRING_GENERATOR(GL_STATIC_DRAW);
        GFX_TO_STRING_GENERATOR(GL_STATIC_READ);
        GFX_TO_STRING_GENERATOR(GL_STATIC_COPY);
        GFX_TO_STRING_GENERATOR(GL_DYNAMIC_DRAW);
        GFX_TO_STRING_GENERATOR(GL_DYNAMIC_READ);
        GFX_TO_STRING_GENERATOR(GL_DYNAMIC_COPY);

        // errors:
        GFX_TO_STRING_GENERATOR(GL_NO_ERROR);
        GFX_TO_STRING_GENERATOR(GL_INVALID_ENUM);
        GFX_TO_STRING_GENERATOR(GL_INVALID_VALUE);
        GFX_TO_STRING_GENERATOR(GL_INVALID_OPERATION);
        GFX_TO_STRING_GENERATOR(GL_INVALID_FRAMEBUFFER_OPERATION);
        GFX_TO_STRING_GENERATOR(GL_OUT_OF_MEMORY);
        GFX_TO_STRING_GENERATOR(GL_STACK_UNDERFLOW);
        GFX_TO_STRING_GENERATOR(GL_STACK_OVERFLOW);

        // types:
        GFX_TO_STRING_GENERATOR(GL_BYTE);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_BYTE);
        GFX_TO_STRING_GENERATOR(GL_SHORT);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_SHORT);
        GFX_TO_STRING_GENERATOR(GL_FLOAT);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_VEC2);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_VEC3);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_VEC4);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_VEC2);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_VEC3);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_VEC4);
        GFX_TO_STRING_GENERATOR(GL_INT);
        GFX_TO_STRING_GENERATOR(GL_INT_VEC2);
        GFX_TO_STRING_GENERATOR(GL_INT_VEC3);
        GFX_TO_STRING_GENERATOR(GL_INT_VEC4);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_VEC2);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_VEC3);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_VEC4);
        GFX_TO_STRING_GENERATOR(GL_BOOL);
        GFX_TO_STRING_GENERATOR(GL_BOOL_VEC2);
        GFX_TO_STRING_GENERATOR(GL_BOOL_VEC3);
        GFX_TO_STRING_GENERATOR(GL_BOOL_VEC4);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT2);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT3);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT4);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT2x3);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT2x4);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT3x2);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT3x4);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT4x2);
        GFX_TO_STRING_GENERATOR(GL_FLOAT_MAT4x3);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT2);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT3);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT4);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT2x3);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT2x4);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT3x2);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT3x4);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT4x2);
        GFX_TO_STRING_GENERATOR(GL_DOUBLE_MAT4x3);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_1D);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_3D);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_CUBE);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_1D_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_1D_ARRAY_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_ARRAY_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_CUBE_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_SAMPLER_2D_RECT_SHADOW);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_1D);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_2D);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_3D);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_CUBE);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_INT_SAMPLER_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_1D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_2D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_3D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_CUBE);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_SAMPLER_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_1D);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_2D);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_3D);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_CUBE);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_IMAGE_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_1D);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_2D);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_3D);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_CUBE);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_1D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_2D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_3D);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_2D_RECT);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_CUBE);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_BUFFER);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_1D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_2D_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
        GFX_TO_STRING_GENERATOR(GL_UNSIGNED_INT_ATOMIC_COUNTER);
    }

    static char buffer[32];
    std::sprintf(buffer, "Unknown GLenum: (0x%04x)", e);
    return buffer;

#undef GFX_TO_STRING_GENERATOR
}

#define GL_CALL(glfunc)                                                                                                                                             \
{                                                                                                                                                                       \
    while (glGetError() != GL_NO_ERROR) {}                                                                                                                              \
    glfunc;                                                                                                                                                             \
    while (GLenum errc = glGetError())                                                                                                                                  \
    {                                                                                                                                                                   \
        std::printf("(0x%04x): %s", errc, GLEnumToString(errc));        \
        std::exit(EXIT_FAILURE);                                                                                                                             \
    }                                                                                                                                                                   \
}



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
            tWrap = GL_REPEAT;

        TextureSamplingOptions() = default;
        TextureSamplingOptions(
            GLenum filterMode, GLenum wrapMode
        ) : minFilter{ filterMode },
            maxFilter{ filterMode },
            sWrap{ wrapMode },
            tWrap{ wrapMode }
        {}

        TextureSamplingOptions(
            GLenum minFilterMode,
            GLenum maxFilterMode,
            GLenum sWrapMode,
            GLenum tWrapMode
        ) : minFilter{ minFilterMode },
            maxFilter{ maxFilterMode },
            sWrap{ sWrapMode },
            tWrap{ tWrapMode }
        {}
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
        virtual void setSamplingOptions(TextureSamplingOptions options) = 0;
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
            //glBindTextureUnit(slot, id);

            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, id);
        }
        inline void unbind() const override {
            glBindTexture(TextureType, 0);
        }

        inline void setSamplingOptions(TextureSamplingOptions options) override
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
