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


}
