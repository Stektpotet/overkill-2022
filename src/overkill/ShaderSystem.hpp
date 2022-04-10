#pragma once
#include <array>
#include <vector>
#include <unordered_map>

#include <overkill/graphics_internal/ShaderProgram.hpp>
#include <overkill/graphics_internal/UniformBuffer.hpp>
#include <overkill/io.hpp>

namespace OK
{
    class ShaderFiles
    {
        std::unordered_map<std::string, std::string> vertex_shaders;
        std::unordered_map<std::string, std::string> fragment_shaders;
        std::unordered_map<std::string, std::string> geometry_shaders;
        std::unordered_map<std::string, std::string> tess_ctrl_shaders;
        std::unordered_map<std::string, std::string> tess_eval_shaders;
        std::unordered_map<std::string, std::string> compute_shaders;

    public:
        bool getSrc(const char * name, GLenum type, std::string * str);
        static ShaderFiles enlistShaderFiles(const char * root);
    };


    class ShaderSystem
    {
        ShaderFiles files;
        static constexpr char errShaderSource[] = ""; // TODO make a minimal passing error-shader
        //ShaderProgram errShader;

        std::unordered_map<std::string, ShaderProgram> computeShaders;
        std::unordered_map<std::string, ShaderProgram> shaders;
        std::unordered_map<std::string, UniformBuffer> uniformBuffers;

    public:
        ShaderSystem(const char* shader_root);

        ShaderProgram* get(const char* name);
        ShaderProgram push(const char* name, const std::initializer_list<std::pair<GLenum, const char*>> src);



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
