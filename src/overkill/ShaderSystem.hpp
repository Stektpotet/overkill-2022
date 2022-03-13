#pragma once

#include<vector>
#include<unordered_map>

#include<graphics_internal/ShaderProgram.hpp>

namespace OK
{

    class ShaderSystem
    {
        std::unordered_map<const char*, ShaderProgram> shaders;
    };

}