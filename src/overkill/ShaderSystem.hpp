#pragma once

#include <array>
#include <vector>
#include <unordered_map>

#include <overkill/graphics_internal/ShaderProgram.hpp>
#include <overkill/graphics_internal/UniformBuffer.hpp>
#include <overkill/io.hpp>

namespace OK
{
    class ShaderSystem
    {
        static constexpr char errShaderSource[] = ""; // TODO make a minimal passing error-shader
        //ShaderProgram errShader;

        std::unordered_map<std::string, ShaderProgram> computeShaders;
        std::unordered_map<std::string, ShaderProgram> shaders;
        std::unordered_map<std::string, UniformBuffer> uniformBuffers;

    public:
        ShaderSystem();

        ShaderProgram* get(const char* name);
        ShaderProgram push(const char* name, const ShaderSources& src);
        UniformBuffer push(UniformBuffer ubo);

        template<GLenum DrawMode>
        UniformBuffer makeUniformBuffer(const char* name, const BlockLayout& layout)
        {
            auto ubo = UniformBuffer(name, layout, DrawMode);
            uniformBuffers.emplace(std::make_pair(name, ubo));
            return ubo;
        }

        void bindUniformBlocks();
    };

}