#include "VertexBuffer.hpp"
namespace OK
{

VertexBuffer::VertexBuffer(const GLsizeiptr size, const void * data /*= nullptr*/)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const GLsizeiptr size, std::initializer_list<std::pair<const GLsizeiptr,const void*>> data/*= nullptr*/)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    GLsizeiptr lastSize = 0;
	for (const auto& v : data) 
	{ //TODO: batch on CPU? -if benefitial at all...
		glBufferSubData(GL_ARRAY_BUFFER, lastSize, v.first, v.second);
		lastSize = v.first;
	}
}
//auto 
//VertexBuffer::resize(const GLsizeiptr size) -> void
//{
//	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
//}

auto 
VertexBuffer::update(const GLintptr start, const GLsizeiptr size, const void * data) -> void
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, start, size, data);
}

//TODO: move the restore pattern into a BufferUtility static class :thinking:
VertexBuffer::VertexBuffer(GLuint id) : id{id}
{}
auto
VertexBuffer::restore(GLuint id) -> VertexBuffer
{
    return VertexBuffer(id);
}

}
