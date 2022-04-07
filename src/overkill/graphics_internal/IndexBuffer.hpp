#pragma once
#include <glad/glad.h>

namespace OK
{

template<typename TIndex>
class IndexBuffer 
{
private:
    GLenum index_type;
	GLuint id;
public:

	IndexBuffer(const GLsizeiptr count, const TIndex* data = nullptr)
        : count{ count }
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(TIndex), data, GL_STATIC_DRAW);
        //GFX_DEBUG("IBO (%d) bound!", id);
	}

	inline bool valid() { return id != 0; }

	inline void bind() const 
    { 
        //GFX_DEBUG("IBO (%d) bound!", id); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); 
    }
	static inline void unbind() 
    { 
        //GFX_DEBUG("IBO unbound!"); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }
	inline void clear() 
    { 
        //GFX_DEBUG("IBO (%d) deleting...", id);
        glDeleteBuffers(1, &id); 
    }
    inline void update(const GLintptr start, const GLsizeiptr size, const void * data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, size, data);
    }
	const GLsizeiptr count;
};

template<>
class IndexBuffer<std::uint8_t>
{
private:
    GLenum index_type = GL_UNSIGNED_BYTE;
    GLuint id;
public:

    IndexBuffer(const GLsizeiptr count, const std::uint8_t* data = nullptr)
        : count{ count }
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint8_t), data, GL_STATIC_DRAW);
        //GFX_DEBUG("IBO (%d) bound!", id);
    }

    inline bool valid() { return id != 0; }

    inline void bind() const
    {
        //GFX_DEBUG("IBO (%d) bound!", id); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
    static inline void unbind()
    {
        //GFX_DEBUG("IBO unbound!"); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    inline void clear()
    {
        //GFX_DEBUG("IBO (%d) deleting...", id);
        glDeleteBuffers(1, &id);
    }
    inline void update(const GLintptr start, const GLsizeiptr size, const void* data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, size, data);
    }
    const GLsizeiptr count;
};

template<>
class IndexBuffer<std::uint16_t>
{
private:
    GLenum index_type = GL_UNSIGNED_SHORT;
    GLuint id;
public:

    IndexBuffer(const GLsizeiptr count, const std::uint16_t* data = nullptr)
        : count{ count }
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint16_t), data, GL_STATIC_DRAW);
        //GFX_DEBUG("IBO (%d) bound!", id);
    }

    inline bool valid() { return id != 0; }

    inline void bind() const
    {
        //GFX_DEBUG("IBO (%d) bound!", id); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
    static inline void unbind()
    {
        //GFX_DEBUG("IBO unbound!"); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    inline void clear()
    {
        //GFX_DEBUG("IBO (%d) deleting...", id);
        glDeleteBuffers(1, &id);
    }
    inline void update(const GLintptr start, const GLsizeiptr size, const void* data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, size, data);
    }
    const GLsizeiptr count;
};


template<>
class IndexBuffer<std::uint32_t>
{
private:
    GLenum index_type = GL_UNSIGNED_INT;
    GLuint id;
public:

    IndexBuffer(const GLsizeiptr count, const std::uint8_t* data = nullptr)
        : count{ count }
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint8_t), data, GL_STATIC_DRAW);
        //GFX_DEBUG("IBO (%d) bound!", id);
    }

    inline bool valid() { return id != 0; }

    inline void bind() const
    {
        //GFX_DEBUG("IBO (%d) bound!", id); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
    static inline void unbind()
    {
        //GFX_DEBUG("IBO unbound!"); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    inline void clear()
    {
        //GFX_DEBUG("IBO (%d) deleting...", id);
        glDeleteBuffers(1, &id);
    }
    inline void update(const GLintptr start, const GLsizeiptr size, const void* data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, size, data);
    }
    const GLsizeiptr count;
};

}
