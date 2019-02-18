#pragma once

#include "lodepng.h"
#include <vector>
#include <string>

typedef struct PNGImage {
	unsigned int width, height;
	std::vector<unsigned char> pixels;
} PNGImage;

PNGImage loadPNGFile(std::string fileName);