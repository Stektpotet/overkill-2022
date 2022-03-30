#include "ShaderProgram.hpp"

namespace OK
{

ShaderProgram::ShaderProgram(UnlinkedShaderProgram unlinked) : id(unlinked.id)
{
	GLint variableCount;
	GLint maxNameLength;

	//get count of active attributes
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &variableCount);
	// Read the length of the longest active uniform name.
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);

	std::string name;
	name.resize(maxNameLength);

	for (size_t i = 0; i < variableCount; i++)
	{
		GLint ignored;
		GLenum type;
		glGetActiveAttrib(id, i, maxNameLength, nullptr, &ignored, &type, (GLchar*)name.data());

		addAttributeLocation(name.data());
	}

	//get count of active uniforms
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &variableCount);
	// Read the length of the longest active uniform name.
	glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

	name.clear();
	name.resize(maxNameLength);
	for (size_t i = 0; i < variableCount; i++)
	{
		GLint ignored;
		GLenum type;
		glGetActiveUniform(id, i, maxNameLength, nullptr, &ignored, &type, (GLchar*)name.data());

		addUniformLocation(name.data());
	}
}

void
ShaderProgram::bind() const
{
	glUseProgram(id);
}

void 
ShaderProgram::unbind() const
{
	glUseProgram(0);
}

void 
ShaderProgram::destroy()
{
	attributes.clear();
	uniforms.clear();
	unbind();
	glDeleteProgram(id);
}

void
ShaderProgram::addUniformLocation( const char* name )
{	// Compiler says there is no try_emplace back for unorderer maps. According to stack overflow, 
    // it wouldn't make sense to have one (https://stackoverflow.com/questions/53772218/why-is-try-emplace-not-implemented-for-stdmultimap).
	uniforms.emplace(name, glGetUniformLocation(id, name));
}

GLint 
ShaderProgram::getUniformLocation( const char* name ) const
{
	auto element = uniforms.find(name);
	if (element != uniforms.end())
		return element->second;
	return -1;
}

void
ShaderProgram::addAttributeLocation( const char* name )
{
	attributes.emplace(name, glGetAttribLocation(id, name));
}

GLint 
ShaderProgram::getAttributeLocation( const char* name ) const
{
	auto element = attributes.find(name);
	if (element != attributes.end())
		return element->second;
	return -1;
}

UnlinkedShaderProgram::UnlinkedShaderProgram()
{
    id = glCreateProgram();
}

UnlinkedShaderProgram::UnlinkedShaderProgram(const ShaderSources & src)
{
    id = glCreateProgram();
    for (size_t i = 0; i < src.data.size(); i++)
    {
        attachShader(src.data[i].data(), src.shaderTypes[i]);
    }
}

GLuint UnlinkedShaderProgram::attachShader(std::string_view src, GLenum shaderType)
{
    if (src.empty())
        return 0;

    GLenum err = 0;
    GLuint shaderId = glCreateShader(shaderType);

    const char* src_data = src.data();

    glShaderSource(shaderId, 1, &src_data, nullptr);
    glCompileShader(shaderId);

    err = ShaderIntrospector::checkCompileStatus(shaderId);
    if (err)
        return err;

    glAttachShader(id, shaderId);
    glDeleteShader(shaderId);               //it may be that I'm supposed to call glDetachShader() instead.. not sure for now tho
    return 0;
}

ShaderSources::ShaderSources(std::initializer_list<std::pair<GLenum, const char*>> sourceFiles)
{
    for (const auto&[type, filePath] : sourceFiles) 
    {
        switch (type)
        {
        case GL_VERTEX_SHADER:
            set<GL_VERTEX_SHADER>(filePath);
            break;
        case GL_FRAGMENT_SHADER:
            set<GL_FRAGMENT_SHADER>(filePath); 
            break;
        case GL_GEOMETRY_SHADER:
            set<GL_GEOMETRY_SHADER>(filePath); 
            break;
        case GL_TESS_CONTROL_SHADER:
            set<GL_TESS_CONTROL_SHADER>(filePath);
            break;
        case GL_TESS_EVALUATION_SHADER:
            set<GL_TESS_EVALUATION_SHADER>(filePath);
            break;
        case GL_COMPUTE_SHADER:
            set<GL_COMPUTE_SHADER>(filePath);
            break;
        default:
            break;  
        }
    }
}

}