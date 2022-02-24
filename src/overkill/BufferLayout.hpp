#pragma once

#include <vector>

#include <glad/glad.h>

namespace OK
{

enum class BufferLayoutRules { STD140 };

struct BufferElement 
{
    GLuint type;
    GLuint count;
    GLboolean normalized;

	template<BufferLayoutRules layoutRule>
	const constexpr GLuint size() {
		return sizeofType<layoutRule>(type) * count;
	}

private:
	template<BufferLayoutRules layoutRule>
	constexpr GLuint sizeofType(GLuint type)
	{
	    static_assert(false)
	}
	template<>
	constexpr static GLuint sizeofType<BufferLayoutRules::STD140>(const GLuint type)
	{
	    switch (type)
	    {
			case GL_FLOAT										: return sizeof(GLfloat);
			case GL_FLOAT_VEC2									: return sizeof(GLfloat)*2;
			case GL_FLOAT_VEC3									: // STD140 vec3 occupies the same space as vec4
			case GL_FLOAT_VEC4									: return sizeof(GLfloat)*4;
			case GL_DOUBLE										: return sizeof(GLdouble);
			case GL_DOUBLE_VEC2									: return sizeof(GLdouble)*2;
			case GL_DOUBLE_VEC3									: // STD140 vec3 occupies the same space as vec4
			case GL_DOUBLE_VEC4									: return sizeof(GLdouble)*4;
			case GL_INT											: return sizeof(GLint);
			case GL_INT_VEC2									: return sizeof(GLint)*2;
			case GL_INT_VEC3									: // STD140 vec3 occupies the same space as vec4
			case GL_INT_VEC4									: return sizeof(GLint)*4;
			case GL_UNSIGNED_INT								: return sizeof(GLuint);
			case GL_UNSIGNED_INT_VEC2							: return sizeof(GLuint)*2;
			case GL_UNSIGNED_INT_VEC3							: // STD140 vec3 occupies the same space as vec4
			case GL_UNSIGNED_INT_VEC4							: return sizeof(GLuint)*4;
			case GL_BOOL										: return sizeof(GLboolean);
			case GL_BOOL_VEC2									: return sizeof(GLboolean)*2;
			case GL_BOOL_VEC3									: // STD140 vec3 occupies the same space as vec4
			case GL_BOOL_VEC4									: return sizeof(GLboolean)*4;
			//case GL_FLOAT_MAT2									: return sizeof(GLfloat)*2*2;
			//case GL_FLOAT_MAT3									: return sizeof(GLfloat)*3*3;
			//case GL_FLOAT_MAT4									: return sizeof(GLfloat)*4*4;
			//case GL_FLOAT_MAT2x3								: return sizeof(GLfloat)*2*3;
			//case GL_FLOAT_MAT2x4								: return sizeof(GLfloat)*2*4;
			//case GL_FLOAT_MAT3x2								: return sizeof(GLfloat)*3*2;
			//case GL_FLOAT_MAT3x4								: return sizeof(GLfloat)*3*4;
			//case GL_FLOAT_MAT4x2								: return sizeof(GLfloat)*4*2;
			//case GL_FLOAT_MAT4x3								: return sizeof(GLfloat)*4*3;
			//case GL_DOUBLE_MAT2									: return sizeof(GLdouble)*2*2;
			//case GL_DOUBLE_MAT3									: return sizeof(GLdouble)*3*3;
			//case GL_DOUBLE_MAT4									: return sizeof(GLdouble)*4*4;
			//case GL_DOUBLE_MAT2x3								: return sizeof(GLdouble)*2*3;
			//case GL_DOUBLE_MAT2x4								: return sizeof(GLdouble)*2*4;
			//case GL_DOUBLE_MAT3x2								: return sizeof(GLdouble)*3*2;
			//case GL_DOUBLE_MAT3x4								: return sizeof(GLdouble)*3*4;
			//case GL_DOUBLE_MAT4x2								: return sizeof(GLdouble)*4*2;
			//case GL_DOUBLE_MAT4x3								: return sizeof(GLdouble)*4*3;
			//case GL_SAMPLER_1D									: 
			//case GL_SAMPLER_2D									: 
			//case GL_SAMPLER_3D									: 
			//case GL_SAMPLER_CUBE								: 
			//case GL_SAMPLER_1D_SHADOW							: 
			//case GL_SAMPLER_2D_SHADOW							: 
			//case GL_SAMPLER_1D_ARRAY							: 
			//case GL_SAMPLER_2D_ARRAY							: 
			//case GL_SAMPLER_1D_ARRAY_SHADOW						: 
			//case GL_SAMPLER_2D_ARRAY_SHADOW						: 
			//case GL_SAMPLER_2D_MULTISAMPLE						: 
			//case GL_SAMPLER_2D_MULTISAMPLE_ARRAY				: 
			//case GL_SAMPLER_CUBE_SHADOW							: 
			//case GL_SAMPLER_BUFFER								: 
			//case GL_SAMPLER_2D_RECT								: 
			//case GL_SAMPLER_2D_RECT_SHADOW						: 
			//case GL_INT_SAMPLER_1D								: 
			//case GL_INT_SAMPLER_2D								: 
			//case GL_INT_SAMPLER_3D								: 
			//case GL_INT_SAMPLER_CUBE							: 
			//case GL_INT_SAMPLER_1D_ARRAY						: 
			//case GL_INT_SAMPLER_2D_ARRAY						: 
			//case GL_INT_SAMPLER_2D_MULTISAMPLE					: 
			//case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY			: 
			//case GL_INT_SAMPLER_BUFFER							: 
			//case GL_INT_SAMPLER_2D_RECT							: 
			//case GL_UNSIGNED_INT_SAMPLER_1D						: 
			//case GL_UNSIGNED_INT_SAMPLER_2D						: 
			//case GL_UNSIGNED_INT_SAMPLER_3D						: 
			//case GL_UNSIGNED_INT_SAMPLER_CUBE					: 
			//case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY				: 
			//case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY				: 
			//case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE			: 
			//case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	: 
			//case GL_UNSIGNED_INT_SAMPLER_BUFFER					: 
			//case GL_UNSIGNED_INT_SAMPLER_2D_RECT				: 
			//case GL_IMAGE_1D									: 
			//case GL_IMAGE_2D									: 
			//case GL_IMAGE_3D									: 
			//case GL_IMAGE_2D_RECT								: 
			//case GL_IMAGE_CUBE									: 
			//case GL_IMAGE_BUFFER								: 
			//case GL_IMAGE_1D_ARRAY								: 
			//case GL_IMAGE_2D_ARRAY								: 
			//case GL_IMAGE_2D_MULTISAMPLE						: 
			//case GL_IMAGE_2D_MULTISAMPLE_ARRAY					: 
			//case GL_INT_IMAGE_1D								: 
			//case GL_INT_IMAGE_2D								: 
			//case GL_INT_IMAGE_3D								: 
			//case GL_INT_IMAGE_2D_RECT							: 
			//case GL_INT_IMAGE_CUBE								: 
			//case GL_INT_IMAGE_BUFFER							: 
			//case GL_INT_IMAGE_1D_ARRAY							: 
			//case GL_INT_IMAGE_2D_ARRAY							: 
			//case GL_INT_IMAGE_2D_MULTISAMPLE					: 
			//case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY				: 
			//case GL_UNSIGNED_INT_IMAGE_1D						: 
			//case GL_UNSIGNED_INT_IMAGE_2D						: 
			//case GL_UNSIGNED_INT_IMAGE_3D						: 
			//case GL_UNSIGNED_INT_IMAGE_2D_RECT					: 
			//case GL_UNSIGNED_INT_IMAGE_CUBE						: 
			//case GL_UNSIGNED_INT_IMAGE_BUFFER					: 
			//case GL_UNSIGNED_INT_IMAGE_1D_ARRAY					: 
			//case GL_UNSIGNED_INT_IMAGE_2D_ARRAY					: 
			//case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE			: 
			//case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY		: 
			//	return sizeof(GLint);
			//case GL_UNSIGNED_INT_ATOMIC_COUNTER:    return sizeof(GLuint);
	  //      case GL_BYTE:                           return sizeof(GLbyte);
	  //      case GL_UNSIGNED_BYTE:                  return sizeof(GLubyte);
	  //      case GL_SHORT:                          return sizeof(GLshort);
	  //      case GL_UNSIGNED_SHORT:                 return sizeof(GLushort);
	  //      case GL_INT_2_10_10_10_REV:             return sizeof(GLint);
	  //      case GL_UNSIGNED_INT_2_10_10_10_REV:    return sizeof(GLuint);
	  //      case GL_NONE:                           return 1;
	    }
		return 0;
		//static_assert(false, "Unsupported Buffer Layout element!");
	}



};

template<BufferLayoutRules layoutRule = BufferLayoutRules::STD140>
class BufferLayout
{
private:
	std::vector<BufferElement> elements;
	GLuint stride;

public:
	BufferLayout(std::initializer_list<BufferElement> layout) : stride { 0u }
	{
		for (BufferElement elem : layout) {
			elements.emplace_back(elem);
			stride += elem.size<layoutRule>();
		}
	}

};

}