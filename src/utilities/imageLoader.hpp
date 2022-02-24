#pragma once

#include "lodepng.h"
#include "overkill/Texture.hpp"
#include <vector>
#include <string>

OK::RawTexture loadPNGFile(std::string fileName);

void savePNGFile(std::string fileName, const OK::RawTexture image);
