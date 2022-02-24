//#include "UniformBuffer.hpp"
//namespace OK
//{
//
//UniformBuffer::UniformBuffer(const char* name, const std::initializer_list<std::pair<const char*, int>>)
//{
//    glGenBuffers(1, &m_id);
//    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
//
//
//    glBufferData(GL_UNIFORM_BUFFER, m_blockLayout.size(), nullptr, drawMode);
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//}
//BlockLayout::BlockLayout(const char* name) : m_blockSize(0), m_name(name) {}
//
//BlockLayout::operator std::string() const
//{
//    return m_name;
//}
//
////UniformBuffer::UniformBuffer(const char* name, const BlockLayout& layout, const GLenum drawMode)
////    : m_name(name)
////    , m_blockLayout(layout)
////{
////    glGenBuffers(1, &m_id);
////    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
////    glBufferData(GL_UNIFORM_BUFFER, m_blockLayout.size(), nullptr, drawMode);
////    glBindBuffer(GL_UNIFORM_BUFFER, 0);
////}
//
//UniformBuffer::operator std::string() const
//{
//    return m_name;
//}
//
//GLuint UniformBuffer::get() const
//{
//    return m_id;
//}
//
//UniformBuffer::operator GLuint() const
//{
//    return m_id;
//}
//
//void UniformBuffer::clean()
//{
//    glDeleteBuffers(1, &m_id);
//}
//
//void UniformBuffer::bind() const
//{
//    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
//}
//
//void UniformBuffer::unbind() const
//{
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//}
//
//GLuint UniformBuffer::getUniformIndex(const std::string& name) const
//{
//    return m_blockLayout.indexOfUniform(name);
//}
//
//void UniformBuffer::update(const GLintptr index, GLsizeiptr size, const void* data)
//{
//    bind();
//    glBufferSubData(GL_UNIFORM_BUFFER, index, size, data);
//}
//
//}