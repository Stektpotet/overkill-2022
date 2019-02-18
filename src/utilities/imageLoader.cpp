#include "imageLoader.hpp"
#include <iostream>

// Original source: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
PNGImage loadPNGFile(std::string fileName)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> pixels; //the raw pixels
	unsigned int width, height;

	//load and decode
	unsigned error = lodepng::load_file(png, fileName);
	if(!error) error = lodepng::decode(pixels, width, height, png);

	//if there's an error, display it
	if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

	// Unfortunately, images usually have their origin at the top left.
	// OpenGL instead defines the origin to be on the _bottom_ left instead, so
	// here's the world's most inefficient way to flip the image vertically.

	// You're welcome :)

	unsigned int widthBytes = 4 * width;

	for(unsigned int row = 0; row < (height / 2); row++) {
		for(unsigned int col = 0; col < widthBytes; col++) {
			std::swap(pixels[row * widthBytes + col], pixels[(height - 1 - row) * widthBytes + col]);
		}
	}

	PNGImage image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;

	return image;

}