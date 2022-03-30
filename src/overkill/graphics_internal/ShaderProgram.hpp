#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <array>

#include "ShaderIntrospector.hpp"
#include <overkill/io.hpp>

namespace OK
{
struct ShaderSources
{

    std::array<std::string, 6> data;
    GLenum shaderTypes[6] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_COMPUTE_SHADER };

    ShaderSources(std::initializer_list<std::pair<GLenum, const char*>> sourceFiles = {});

    template<GLenum ShaderType>
    void set(const char* filePath);

 
    template<>
    void set<GL_VERTEX_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[0]); }
    template<>
    void set<GL_FRAGMENT_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[1]); }
    template<>
    void set<GL_GEOMETRY_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[2]); }
    template<>
    void set<GL_TESS_CONTROL_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[3]); }
    template<>
    void set<GL_TESS_EVALUATION_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[4]); }
    template<>
    void set<GL_COMPUTE_SHADER>(const char* filePath) { IO::fileToString(filePath, &data[5]); }
};


class UnlinkedShaderProgram 
{
    friend class ShaderSystem;
    friend class ShaderProgram;
    GLuint id;

    UnlinkedShaderProgram();
    UnlinkedShaderProgram(const ShaderSources& src);
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

class ShaderProgram 
{
public:
	ShaderProgram(GLuint i);
	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram&) = default;

	explicit inline operator GLuint() const { return id; }

	void bind();
	void unbind();
	void destroy();

	inline GLuint ID() { return id; }

	GLint getAttributeLocation(const char* name) const;
	GLint getUniformLocation(const char* name) const;

private:
	void  addAttributeLocation(const char* name);
	void  addUniformLocation(const char* name);

	//caching mechanism
	std::unordered_map<std::string, GLint> attributes;
	std::unordered_map<std::string, GLint> uniforms;

	GLuint id;
};
