#pragma once
#include <glad/glad.h>
#include <vector>

#include "VertexBuffer.hpp"

struct Attribute 
{
	const char* name;
	const GLuint size;
	const GLint componentCount;
	const GLenum type;
	const GLboolean normalized = GL_FALSE;

	Attribute(
		const char* name,
		const GLuint size,
		const GLint componentCount,
		const GLenum type,
		const GLboolean normalized = GL_FALSE)
		:	
        name{ name },
        size{ size },
        componentCount{ componentCount },
        type{ type },
        normalized{ normalized }
	{}
};


class VertexLayout 
{
private:
	std::vector<Attribute> m_attributes;
protected:
	GLuint m_stride = 0;
    virtual void addStride(GLuint byteSize) = 0;

    VertexLayout();
    VertexLayout(std::initializer_list<Attribute> attributes);

public:

    void applyToBuffer(const VertexBuffer& buffer);

    //NOTE: For some reason, Gfx::GLTypeSize is not reccognized every time, 
    //      making the template very tedious to write out. A new way to write 
    //      layouts is on the way!
    //      (see VertexLayout(LayoutInitializerList initializer))
/*
	template<
		GLenum		 type,
		GLuint		 componentCount,
		GLint		 count = 1,
		GLboolean	 normalized = GL_FALSE,
		bool		 packed = false,
		GLint		 byteSize = (Gfx::GLTypeSize(type) * (packed ? 1 : componentCount) * count)
	>
	void pushAttrib(const char* name)
	{
		GFX_ASSERT((1 <= componentCount && componentCount <= 4) || componentCount == GL_BGRA, "pushing attribute with illegal count: %d (accepted values: 1-4, GL_BGRA)", componentCount);
        m_attributes.push_back(Attribute(name, byteSize, componentCount, type, normalized));
		addStride(byteSize);
	}

	template<
		GLenum		 type,
		GLuint		 componentCount,
		GLint		 count = 1,
		GLboolean	 normalized = GL_TRUE,
		GLint		 byteSize = Gfx::GLTypeSize(type) * count>
	auto pushAttribPacked(const char* name) -> decltype(pushAttrib<type, componentCount, count, normalized, true, byteSize>(name))
	{
		return pushAttrib<type, componentCount, count, normalized, true, byteSize>(name);
	}*/
};

class ContinuousVertexLayout final : public VertexLayout
{
public: 
    ContinuousVertexLayout(std::initializer_list<Attribute> initializer);
protected:
    inline void addStride(GLuint byteSize) final;
};

class InterleavingVertexLayout final : public VertexLayout
{
public:
    InterleavingVertexLayout(std::initializer_list<Attribute> initializer);
protected:
    inline void addStride(GLuint byteSize) final;
};