#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <array>

#include "ShaderIntrospector.hpp"
#include <overkill/io.hpp>

namespace OK
{

class UnlinkedShaderProgram 
{
    friend class ShaderSystem;
    friend class ShaderProgram;
    GLuint id;

    UnlinkedShaderProgram();
    //UnlinkedShaderProgram(const ShaderSources& src);
public:

    GLuint attachShader(std::string_view src, GLenum shaderType);
};

class ShaderProgram 
{
private:
    friend class ShaderSystem; // Give access to creation of shaders from the shadersystem

    GLuint id;
    //caching mechanism
    std::unordered_map<std::string, GLint> attributes;
    std::unordered_map<std::string, GLint> uniforms;

    ShaderProgram(UnlinkedShaderProgram unlinkedProgramID);
public:
    ShaderProgram(const ShaderProgram&) = default;

	explicit inline operator GLuint() const { return id; }

	void bind() const;
	void unbind() const;
	void destroy();

	inline GLuint ID() const { return id; }

	GLint getAttributeLocation(const char* name) const;
	GLint getUniformLocation(const char* name) const;

private:
	void  addAttributeLocation(const char* name);
	void  addUniformLocation(const char* name);
};

}

