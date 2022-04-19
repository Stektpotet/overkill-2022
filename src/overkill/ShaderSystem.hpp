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
        friend ShaderSystem;
        std::unordered_map<std::string, std::string> vertex_shaders;
        std::unordered_map<std::string, std::string> fragment_shaders;
        std::unordered_map<std::string, std::string> geometry_shaders;
        std::unordered_map<std::string, std::string> tess_ctrl_shaders;
        std::unordered_map<std::string, std::string> tess_eval_shaders;
        std::unordered_map<std::string, std::string> compute_shaders;

    public:
        bool getSrc(const char * name, GLenum type, std::string * str);
        //static ShaderFiles enlistShaderFiles(const char * root);
    };


    class ShaderSystem
    {
        static ShaderFiles files;
        static constexpr const char errShaderSource[] = ""; // TODO make a minimal passing error-shader
        //ShaderProgram errShader;

        static std::unordered_map<std::string, ShaderProgram*> computeShaders;
        static std::unordered_map<std::string, ShaderProgram*> shaders;
        static std::unordered_map<std::string, UniformBuffer*> uniformBuffers;

    private:
        ShaderSystem() {};

    public:
        static constexpr const char DEFAULT[] = "__DEFAULT__";
        static constexpr const char VIEWPORT_DEFAULT[] = "__VIEWPORT_DEFAULT__";
        static void add_shader_sources(const char* directory);

        static ShaderProgram* const get(const char* name);
        static ShaderProgram* const push(const char* name, const std::initializer_list<std::pair<GLenum, const char*>> src);

        static ShaderProgram* const get_compute(const char* name);
        static ShaderProgram* const push_compute(const char* name, const char* src);
        static ShaderProgram* const push_compute(const char* name);

        static UniformBuffer* const get_uniform_buffer(const char* name);
        
        template<GLenum DrawMode>
        static UniformBuffer* const makeUniformBuffer(const BlockLayout& layout)
        {
            auto name = layout.get_name();
            uniformBuffers.emplace(name, new UniformBuffer(layout, DrawMode));
            return uniformBuffers[name];
        }

        template<GLenum DrawMode>
        static UniformBuffer* makeUniformBuffer(const char* name = "", std::initializer_list<std::pair<const char*, int>> elements = {})
        {
            return makeUniformBuffer<DrawMode>(BlockLayout(name, elements));
        }

        static void bindUniformBlocks();

    };

}
