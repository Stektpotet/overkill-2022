#include "ShaderIntrospector.hpp"
#include <iostream>
namespace OK
{
    auto ShaderIntrospector::getUniformMaxNameLength(const GLuint programId) -> GLsizei
    {
        GLsizei nameMaxLength;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
        return nameMaxLength;
    }

    auto ShaderIntrospector::getAttributeMaxNameLength(const GLuint programId) -> GLsizei
    {
        GLsizei nameMaxLength;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength);
        return nameMaxLength;
    }

    auto ShaderIntrospector::getAttribName(const GLuint programId, const GLuint attribIndex, const GLsizei nameMaxLength) -> std::string
    {
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        GLsizei length;
        GLint size; // size of the variable

        char* attribName = (char*)alloca(nameMaxLength * sizeof(char));
        glGetActiveAttrib(programId, attribIndex, nameMaxLength, &length, &size, &type, attribName);
     //   LOG_DEBUG("Attribute #%u Type: %u Name: %s", attribIndex, type, attribName);
        return std::string(attribName);
    }

    auto ShaderIntrospector::getUniformName(const GLuint programId, const GLuint uniformIndex, const GLsizei nameMaxLength) -> std::string
    {
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        GLint size; // size of the variable
        GLsizei length;

        char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
        glGetActiveUniform(programId, uniformIndex, nameMaxLength, &length, &size, &type, uniformName);
       // LOG_DEBUG("Uniform #%u, Type: %u, Name: %s", uniformIndex, type, uniformName);

        return std::string(uniformName);
    };
// UNIFORMS
/*
const std::vector<GLint>& ShaderIntrospector::getUniformLocations(const GLuint program)
{
    GLsizei nameMaxLength;

    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength));
    LOG_INFO("Uniform name maxlength: %i", nameMaxLength);
    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count));
    LOG_INFO("\n\nActive uniforms: %i", count);
    for (GLuint i = 0; i < count; i++)
    {
        char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
        GLCall(glGetActiveUniform(id, i, nameMaxLength, &length, &size, &type, uniformName));
    }
    GLuint count;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));

    GLint indices;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &indices));
    
    std::vector<GLint> result;
    result.reserve(count);
    for (GLuint i = 0 < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}*/


auto ShaderIntrospector::checkCompileStatus(const GLuint shaderid) -> GLuint
{
    GLint result;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderid, length, &length, message);
        glDeleteShader(shaderid);

        std::cout << "GL_COMPILE_STATUS:" << message << std::endl;
        return 1;
    }
    return 0;
}

auto ShaderIntrospector::checkLinkStatus(const GLuint programid) -> GLuint
{
    GLint result;
    glGetProgramiv(programid, GL_LINK_STATUS, &result);

    if (!result)
    {
        int length;
        glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(programid, length, &length, message);
        glDeleteProgram(programid);

        std::cout << "GL_LINK_STATUS: " << message << std::endl;
        std::cin.get();
        return 1;
    }
    return 0; 
}

//UNIFORM BLOCKS

GLint ShaderIntrospector::getActiveBlockCount(const GLuint program)
{
    GLint count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count);
    return count;
}

const std::string ShaderIntrospector::getUnifromBlockName(const GLuint program, const GLint uBlockIndex)
{
    //nameMaxLength = the longest name of a block for the given program
    GLint nameMaxLength, length;
    glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameMaxLength);
    
    
    char* name = (char*)alloca(nameMaxLength * sizeof(char));
    glGetActiveUniformBlockName(program, uBlockIndex, nameMaxLength, &length, name);
    return std::string(name);
}

const std::vector<GLint> ShaderIntrospector::getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex)
{
    GLint count;
    glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count);

    GLint* indices = (GLint*)alloca(count * sizeof(GLint));
    glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);

    std::vector<GLint> result;
    result.reserve(count);
    for (GLint i = 0; i < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}

GLuint ShaderIntrospector::getUniformBlockIndex(const GLuint program, const std::string& name)
{
    GLuint index;
    index = glGetUniformBlockIndex(program, name.c_str());
    return index;
}

GLint ShaderIntrospector::getActiveUniformCount(const GLuint program)
{
    GLint count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    return count;
}

GLint ShaderIntrospector::getActiveAttribCount(const GLuint program)
{
    GLint count;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    return count;
}

}
