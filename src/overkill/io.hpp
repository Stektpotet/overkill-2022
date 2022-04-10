#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#include <vector>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include <tiny_gltf.h>

namespace OK {
    struct Transform;
}


namespace OK::IO
{

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
