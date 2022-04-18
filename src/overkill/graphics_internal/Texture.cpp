#include "Texture.hpp"
#include <stdio.h>
#include <iostream>


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


}
