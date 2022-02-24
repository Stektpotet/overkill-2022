#include "Texture.hpp"
namespace OK
{
TextureBase::TextureBase(
    uint16_t width, 
    uint16_t height, 
    uint16_t channels
) : id{0u},
    width{width},
    height{height},
    channels{channels}
{}

Texture2DArray::Texture2DArray(
    uint16_t width, 
    uint16_t height, 
    uint16_t depth, 
    uint16_t channels,
    TextureSamplingOptions options
) :	Texture(width, height, channels, options), 
    depth{ depth }
{}
Texture2DArray makeAtlasTexture(RawTexture rawTexture, uint16_t depth, TextureSamplingOptions options) {
    Texture2DArray texture = Texture2DArray(rawTexture.width, rawTexture.height, depth, rawTexture.channels, options);
    glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, rawTexture.width, rawTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawTexture.pixels.data());
    return texture;
}
Texture2D makeTexture(RawTexture rawTexture, TextureSamplingOptions options) {
    Texture2D texture = Texture2D(rawTexture.width, rawTexture.height, rawTexture.channels, options);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rawTexture.width, rawTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawTexture.pixels.data());
    return texture;
}
}