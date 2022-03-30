#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace OK::IO
{

// @ref Inspired by https://stackoverflow.com/a/2602060
//    and http://insanecoding.blogspot.no/2011/11/how-to-read-in-file-in-c.html
/// <summary>
/// reads the <paramref name="file"/> into <paramref name="outString"/>
/// </summary>
/// <param name="file">the path of the file to read</param>
/// <param name="outString">the string to be filled</param>
/// <returns> true when successful, false when unsuccessful </returns>
    bool fileToString(const std::string& filePath, std::string* outString);


    namespace Shader
    {

    struct ShaderFiles 
    {
        std::unordered_map<std::string, std::string> vertex_shaders;
        std::unordered_map<std::string, std::string> fragment_shaders;
        std::unordered_map<std::string, std::string> geometry_shaders;
        std::unordered_map<std::string, std::string> tess_ctrl_shaders;
        std::unordered_map<std::string, std::string> tess_eval_shaders;
        std::unordered_map<std::string, std::string> compute_shaders;
    };

    ShaderFiles enlistShaderFiles(const char* root);

    }
}
