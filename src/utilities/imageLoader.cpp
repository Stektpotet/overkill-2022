#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include "imageLoader.hpp"
#include <iostream>

// Original source: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
OK::RawTexture loadPNGFile(std::string fileName)
{
    stbi_set_flip_vertically_on_load(true);
    std::vector<unsigned char> pixels; //the raw pixels
#ifdef STB_IMAGE_IMPLEMENTATION
    int width, height, comp;

    unsigned char* rawPixels = stbi_load(fileName.c_str(), &width, &height, &comp, STBI_rgb_alpha);
    pixels.insert(pixels.end(), &rawPixels[0], &rawPixels[width * height * 4 - 1]);
    stbi_image_free(rawPixels);
#else

    std::vector<unsigned char> png;
    unsigned int width, height;

    //load and decode
    //unsigned error = lodepng::load_file(png, fileName);
    //if(!error) error = lodepng::decode(pixels, width, height, png);

    unsigned error = lodepng::decode(pixels, width, height, fileName);

    //if there's an error, display it
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

    // Unfortunately, images usually have their origin at the top left.
    // OpenGL instead defines the origin to be on the _bottom_ left instead, so
    // here's the world's most inefficient way to flip the image vertically.

    // You're welcome :)

    unsigned int widthBytes = 4 * width;

    for (unsigned int row = 0; row < (height / 2); row++) {
        for (unsigned int col = 0; col < widthBytes; col++) {
            std::swap(pixels[row * widthBytes + col], pixels[(height - 1 - row) * widthBytes + col]);
        }
    }
#endif

    OK::RawTexture image;
    image.width = width;
    image.height = height;
    image.pixels = pixels;

    return image;

}

void savePNGFile(std::string fileName, OK::RawTexture img)
{
#ifdef STB_IMAGE_WRITE_IMPLEMENTATION

    stbi_write_png("sky.png", img.width, img.height, 4, img.pixels.data(), img.width * 4);
#else 

    std::vector<unsigned char> png;
    unsigned int widthBytes = 4 * img.width;
    for (unsigned int row = 0; row < (img.height / 2); row++) {
        for (unsigned int col = 0; col < widthBytes; col++) {
            std::swap(img.pixels[row * widthBytes + col], img.pixels[(img.height - 1 - row) * widthBytes + col]);
        }
    }

    unsigned error = lodepng::encode(png, img.pixels, img.width, img.height, LodePNGColorType::LCT_RGBA);
    if (!error) lodepng::save_file(png, fileName);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
#endif
}
