#include "ShaderSystem.hpp"
namespace OK
{
    //Forward declare static fields
    ShaderFiles ShaderSystem::files;
    std::unordered_map<std::string, ShaderProgram*> ShaderSystem::computeShaders;
    std::unordered_map<std::string, ShaderProgram*> ShaderSystem::shaders;
    std::unordered_map<std::string, UniformBuffer*> ShaderSystem::uniformBuffers;

    /// <summary>
    /// Add all shader files found in 'directory' by their filepath stem (name)
    /// </summary>
    /// <param name="directory">where to look for shader files</param>
    void ShaderSystem::add_shader_sources(const char* directory)
    {
        for (const auto& entry : std::filesystem::directory_iterator(directory))
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
    }

    //ShaderSystem::ShaderSystem(const char* shaders_root) : shaders{}, computeShaders{} 
    //{
    //    files = ShaderFiles::enlistShaderFiles(shaders_root);
    //}
    ShaderProgram* const ShaderSystem::get(const char* name)
    {
        auto element = shaders.find(name);
        if (element != shaders.end())
            return element->second;
        std::cout << "WARN: Unable to find shader \"" << name << "\" registered in shadersystem, loading default shader instead...";
        element = shaders.find(OK::ShaderSystem::DEFAULT);
        assert(element != shaders.end() && "Default shader doesnt exist!");
        return element->second; // TODO: Return default/error shader
    }

    ShaderProgram* const ShaderSystem::get_compute(const char* name)
    {
        auto element = computeShaders.find(name);
        if (element != computeShaders.end())
            return element->second;
        std::cout << "WARN: Unable to find shader \"" << name << "\" registered in shadersystem, loading default shader instead...";
        element = computeShaders.find(OK::ShaderSystem::DEFAULT);
        assert(element != shaders.end() && "Default shader doesnt exist!");
        return element->second; // TODO: Return default/error shader
    }

    UniformBuffer* const ShaderSystem::get_uniform_buffer(const char* name)
    {
        return uniformBuffers[name];
    }

    ShaderProgram* const ShaderSystem::push_compute(const char* name) {
        return push_compute(name, name);
    }
    ShaderProgram* const ShaderSystem::push_compute(const char* name, const char* src)
    {
        UnlinkedShaderProgram unlinked = UnlinkedShaderProgram();
        std::string source_code;
        if (files.getSrc(src, GL_COMPUTE_SHADER, &source_code))
            unlinked.attachShader(source_code, GL_COMPUTE_SHADER);
        glLinkProgram(unlinked.id);
        ShaderIntrospector::checkLinkStatus(unlinked.id);
        computeShaders.emplace(name, new ShaderProgram(unlinked));
        return computeShaders[name];
    }
    ShaderProgram* const ShaderSystem::push(const char* name, const std::initializer_list<std::pair<GLenum, const char*>> src)
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
        if (ShaderIntrospector::checkLinkStatus(unlinked.id))
            std::cout << "ERR: Failed linking \"" << name << "\"" << std::endl;
        shaders.emplace(name, new ShaderProgram(unlinked));
        return shaders[name];
    }
    
    void ShaderSystem::bindUniformBlocks()
    {
        GLuint bindPoint = 0;
        for (const auto& [uniform_buffer_name, ubo] : uniformBuffers)
        {
            for (const auto& [shader_name, shader] : shaders) {
                auto uBlockIndex = OK::ShaderIntrospector::getUniformBlockIndex(shader->ID(), uniform_buffer_name);
                if (uBlockIndex != GL_INVALID_INDEX) // As long as the block exists in the shader
                {
                    std::cout << "INFO: Binding UBO[" << uBlockIndex << "] and shader [" << shader->ID() << "] in binding point "<< bindPoint << std::endl;
                    glUniformBlockBinding(shader->ID(), uBlockIndex, bindPoint);  //link blocks of the same type to the same binding point
                    glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, ubo->get()); //reserve data range on the uniform buffer
                }
            }
            bindPoint++;
        }
    }

    inline bool ShaderFiles::getSrc(const char * name, GLenum type, std::string * str)
    {
        bool success = false;
        switch (type)
        {
        case GL_VERTEX_SHADER:
            success = IO::read_file_to_string(vertex_shaders[name], str);
            break;
        case GL_FRAGMENT_SHADER:
            success = IO::read_file_to_string(fragment_shaders[name], str);
            break;
        case GL_GEOMETRY_SHADER:
            success = IO::read_file_to_string(geometry_shaders[name], str);
            break;
        case GL_TESS_CONTROL_SHADER:
            success = IO::read_file_to_string(tess_ctrl_shaders[name], str);
            break;
        case GL_TESS_EVALUATION_SHADER:
            success = IO::read_file_to_string(tess_eval_shaders[name], str);
            break;
        case GL_COMPUTE_SHADER:
            success = IO::read_file_to_string(compute_shaders[name], str);
            break;
        default:
            break;
        }
        return success;
    }

    //inline ShaderFiles ShaderFiles::enlistShaderFiles(const char * root)
    //{
    //    ShaderFiles files;
    //    for (const auto & entry : std::filesystem::directory_iterator(root))
    //    {
    //        auto& path = entry.path();
    //        if (path.has_filename() && path.has_extension())
    //        {
    //            auto ext = path.extension();

    //            if (ext == ".vert")
    //                files.vertex_shaders.emplace(path.stem().string(), path.string());
    //            else if (ext == ".frag")
    //                files.fragment_shaders.emplace(path.stem().string(), path.string());
    //            else if (ext == ".geom")
    //                files.geometry_shaders.emplace(path.stem().string(), path.string());
    //            else if (ext == ".tesc")
    //                files.tess_ctrl_shaders.emplace(path.stem().string(), path.string());
    //            else if (ext == ".tese")
    //                files.tess_eval_shaders.emplace(path.stem().string(), path.string());
    //            else if (ext == ".comp")
    //                files.compute_shaders.emplace(path.stem().string(), path.string());
    //        }
    //    }
    //    return files;
    //}

}
