#pragma once

#include "lodepng.h"
#include <vector>
#include <string>

typedef struct PNGImage {
	unsigned int width;
	unsigned int height;
	std::vector<unsigned char> pixels;
} PNGImage;

PNGImage loadPNGFile(std::string fileName);
