#include "Texture.hpp"
#include <stdio.h>
#include <iostream>
#include <program.hpp>


namespace OK
{
TextureBase::TextureBase(
    uint16_t width, 
    uint16_t height, 
    uint16_t channels,
    TextureSamplingOptions options
) : id{0u},
    width{width},
    height{height},
    channels{channels},
    samplingOptions{options}
{}

Texture2DArray::Texture2DArray(
    RawTexture8& raw,
    TextureSamplingOptions options
) :	Texture(raw, options), 
    depth{ depth }
{}


Texture3D::Texture3D(uint16_t width, uint16_t height, uint16_t depth, uint16_t channels, TextureSamplingOptions options)
    : id{ 0u },
    width{ width },
    height{ height },
    depth{ depth },
    channels{ channels },
    samplingOptions{ options }
{
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    printGLError();
    glBindTexture(GL_TEXTURE_3D, id);
    printGLError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, options.sWrap);
    printGLError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, options.tWrap);
    printGLError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, options.rWrap);
    printGLError();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, options.minFilter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, options.maxFilter);
}

void Texture3D::bind(GLuint slot) const {
    glBindTextureUnit(slot, id);
}
void Texture3D::unbind() const {
    glBindTexture(GL_TEXTURE_3D, 0);
}

void  Texture3D::setSamplingOptions(TextureSamplingOptions options)
{
    glBindTexture(GL_TEXTURE_3D, id);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, options.minFilter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, options.maxFilter);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, options.sWrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, options.tWrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, options.rWrap);

}
}
