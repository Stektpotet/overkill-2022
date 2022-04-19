#pragma once

#include "lodepng.h"
#include "overkill/graphics_internal/Texture.hpp"
#include <vector>
#include <string>

OK::RawTexture8 loadPNGFile(std::string fileName);

void savePNGFile(std::string fileName, const OK::RawTexture8 image);
