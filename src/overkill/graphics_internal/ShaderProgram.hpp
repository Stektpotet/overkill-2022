#pragma once
#include <glad/glad.h>
#include <unordered_map>

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