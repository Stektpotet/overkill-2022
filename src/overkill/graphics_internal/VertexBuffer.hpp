#pragma once
#include <vector>
#include <initializer_list>
#include <glad/glad.h>
namespace OK
{
class VertexBuffer 
{
private:
    VertexBuffer(GLuint id);
    GLuint id = 0;
    //VertexLayout& layout;

    //TODO: Make restore constructors which build the buffers up from the GPU
public:
	//GLsizeiptr is used because its capacity is larger than that of GLsizei
    VertexBuffer() = default;
	VertexBuffer(const GLsizeiptr size, const void * data = nullptr);
	VertexBuffer(const GLsizeiptr size, std::initializer_list<std::pair<const GLsizeiptr, const void*>> data);

	inline GLuint ID() const { return id; }

	inline bool valid() { return id != 0; }


	inline void bind() const 
    { 
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }
	static inline void unbind() 
    { 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
    }
	inline void clear() 
    {
        glDeleteBuffers(1, &id);
    }

	void update(const GLintptr start, const GLsizeiptr size, const void * data);

    static VertexBuffer restore(GLuint id);


};

}
