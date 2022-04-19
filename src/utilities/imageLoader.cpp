#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include "imageLoader.hpp"
#include <iostream>

OK::RawTexture8 loadPNGFile(std::string fileName)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, comp;
    unsigned char* rawPixels = stbi_load(fileName.c_str(), &width, &height, &comp, STBI_default);
    
    OK::RawTexture img(width, height, comp, rawPixels);

    stbi_image_free(rawPixels);

    return img;
}

void savePNGFile(std::string fileName, OK::RawTexture8 img)
{
    stbi_flip_vertically_on_write(true);
    stbi_write_png(fileName.data(), img.width, img.height, 4, img.pixels, img.width * 4);
}
