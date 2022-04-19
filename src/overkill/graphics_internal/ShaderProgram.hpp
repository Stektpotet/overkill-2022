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

    //UnlinkedShaderProgram(const ShaderSources& src);
    UnlinkedShaderProgram(); // only friends can make one
public:

    GLuint attachShader(std::string_view src, GLenum shaderType);
};

class ShaderProgram 
{
public: // TODO: make private again -> put as public for debugging purposes
    friend class ShaderSystem; // Give access to creation of shaders from the shadersystem

    GLuint id = 0;
    //caching mechanism
    std::unordered_map<std::string, GLint> attributes;
    std::unordered_map<std::string, GLint> uniforms;

    ShaderProgram(UnlinkedShaderProgram unlinkedProgramID);
public:
    ShaderProgram(const ShaderProgram&) = default;

    ShaderProgram();

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

