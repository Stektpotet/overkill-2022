#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#include <vector>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include <tiny_gltf.h>
#include "stb_image.h"
#include <overkill/graphics_internal/Texture.hpp>

namespace OK {
    struct Transform;
}


namespace OK::IO
{


    namespace Detail
    {
        template<typename TPixel>
        inline TPixel* load_texture(char const* filename, int* x, int* y, int* comp, int req_comp)
        {
            std::cout << "Error: load_texture not implemented for " << typeid(TPixel).name() << std::endl;
        }

        template<>
        inline std::uint8_t* load_texture<std::uint8_t>(char const* filename, int* x, int* y, int* comp, int req_comp)
        {
            return stbi_load(filename, x, y, comp, req_comp);
        }
        template<>
        inline std::uint16_t* load_texture<std::uint16_t>(char const* filename, int* x, int* y, int* comp, int req_comp)
        {
            return stbi_load_16(filename, x, y, comp, req_comp);
        }
    }

    template<typename TPixel>
    RawTexture<TPixel> read_raw_texture(const std::string_view filePath)
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, comp;

        TPixel* rawPixels = Detail::load_texture<TPixel>(filePath.data(), &width, &height, &comp, STBI_default);
        OK::RawTexture<TPixel> img(width, height, comp, rawPixels);
        stbi_image_free(rawPixels);

        return img;
    }


// @ref Inspired by https://stackoverflow.com/a/2602060
//    and http://insanecoding.blogspot.no/2011/11/how-to-read-in-file-in-c.html
/// <summary>
/// reads the <paramref name="file"/> into <paramref name="outString"/>
/// </summary>
/// <param name="file_path">the path of the file to read</param>
/// <param name="out_string">the string to be filled</param>
/// <returns> true when successful, false when unsuccessful </returns>
    bool read_file_to_string(const std::string_view file_path, std::string* out_string);

    bool read_obj_into_scene(const std::string_view file_path, Transform& parent);
    bool read_gltf_into_scene(const std::string_view file_path, Transform& parent);
}
