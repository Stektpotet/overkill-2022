#pragma once
#include <glad/glad.h>

namespace OK
{

template<typename TIndex>
class IndexBuffer 
{
private:
	GLuint id;
public:

	IndexBuffer(const GLsizeiptr count, const TIndex* data = nullptr)
        : count{ count }
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(TIndex), data, GL_STATIC_DRAW);
        GFX_DEBUG("IBO (%d) bound!", id);
	}

	inline bool valid() { return id != 0; }

	inline void bind() const 
    { 
        GFX_DEBUG("IBO (%d) bound!", id); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); 
    }
	static inline void unbind() 
    { 
        GFX_DEBUG("IBO unbound!"); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }
	inline void clear() 
    { 
        GFX_DEBUG("IBO (%d) deleting...", id);
        glDeleteBuffers(1, &id); 
    }
    inline void update(const GLintptr start, const GLsizeiptr size, const void * data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, size, data);
    }
	const GLsizeiptr count;
};

}