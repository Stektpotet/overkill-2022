#include "ShaderSystem.hpp"
namespace OK
{
    ShaderSystem::ShaderSystem() : shaders{}, computeShaders{} {}
    ShaderProgram* ShaderSystem::get(const char* name)
{
    auto element = shaders.find(name);
    if (element != shaders.end())
        return &(element->second);
    return nullptr; // TODO: Return default/error shader
}

ShaderProgram OK::ShaderSystem::push(const char* name, const ShaderSources& src)
{
    UnlinkedShaderProgram unlinked = UnlinkedShaderProgram(src);
    glLinkProgram(unlinked.id);
    ShaderIntrospector::checkLinkStatus(unlinked.id);
    ShaderProgram program = ShaderProgram(unlinked);
    shaders.insert({ name, program });
    return program;
}

UniformBuffer ShaderSystem::push(UniformBuffer ubo)
{
    uniformBuffers.emplace(std::make_pair(ubo.name(), ubo));
    return ubo;
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

}