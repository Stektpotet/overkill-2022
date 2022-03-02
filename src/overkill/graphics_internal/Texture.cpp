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
    RawTexture raw,
    TextureSamplingOptions options
) :	Texture(raw), 
    depth{ depth }
{}
}