#include "ShaderSystem.hpp"
namespace OK
{
    ShaderSystem::ShaderSystem(const char* shaders_root) : shaders{}, computeShaders{} 
    {
        files = ShaderFiles::enlistShaderFiles(shaders_root);
    }
    ShaderProgram* ShaderSystem::get(const char* name)
    {
        auto element = shaders.find(name);
        if (element != shaders.end())
            return &(element->second);
        return nullptr; // TODO: Return default/error shader
    }
    
    ShaderProgram OK::ShaderSystem::push(const char* name, const std::initializer_list<std::pair<GLenum, const char*>> src)
    {
        UnlinkedShaderProgram unlinked = UnlinkedShaderProgram();
        for (const auto& [type, shader_name] : src)
        {
            std::string source_code;
            if (files.getSrc(shader_name, type, &source_code))
            {
                unlinked.attachShader(source_code, type);
            }
        }
    
        glLinkProgram(unlinked.id);
        ShaderIntrospector::checkLinkStatus(unlinked.id);
        ShaderProgram program = ShaderProgram(unlinked);
        shaders.insert({ name, program });
        return program;
    }
    
    void ShaderSystem::bindUniformBlocks()
    {
        for (const auto& [uniform_buffer_name, ubo] : uniformBuffers)
        {
            for (const auto& [shader_name, shader] : shaders) {
                auto uBlockIndex = OK::ShaderIntrospector::getUniformBlockIndex(shader.ID(), uniform_buffer_name);
                if (uBlockIndex != GL_INVALID_INDEX) // As long as the block exists in the shader
                {
                    glUniformBlockBinding(shader.ID(), uBlockIndex, 0);  //link blocks of the same type to the same binding point
                    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo.get()); //reserve data range on the uniform buffer
                }
            }
        }
    }

    inline bool ShaderFiles::getSrc(const char * name, GLenum type, std::string * str)
    {
        bool success = false;
        switch (type)
        {
        case GL_VERTEX_SHADER:
            success = IO::fileToString(vertex_shaders[name], str);
            break;
        case GL_FRAGMENT_SHADER:
            success = IO::fileToString(fragment_shaders[name], str);
            break;
        case GL_GEOMETRY_SHADER:
            success = IO::fileToString(geometry_shaders[name], str);
            break;
        case GL_TESS_CONTROL_SHADER:
            success = IO::fileToString(tess_ctrl_shaders[name], str);
            break;
        case GL_TESS_EVALUATION_SHADER:
            success = IO::fileToString(tess_eval_shaders[name], str);
            break;
        case GL_COMPUTE_SHADER:
            success = IO::fileToString(compute_shaders[name], str);
            break;
        default:
            break;
        }
        return success;
    }

    inline ShaderFiles ShaderFiles::enlistShaderFiles(const char * root)
    {
        ShaderFiles files;
        for (const auto & entry : std::filesystem::directory_iterator(root))
        {
            auto& path = entry.path();
            if (path.has_filename() && path.has_extension())
            {
                auto ext = path.extension();

                if (ext == ".vert")
                    files.vertex_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".frag")
                    files.fragment_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".geom")
                    files.geometry_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".tesc")
                    files.tess_ctrl_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".tese")
                    files.tess_eval_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".comp")
                    files.compute_shaders.emplace(path.stem().string(), path.string());
            }
        }
        return files;
    }

}