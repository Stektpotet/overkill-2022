#include "UniformBuffer.hpp"

namespace OK
{
    BlockLayout::BlockLayout(const char* name, std::initializer_list<std::pair<const char*, int>> elements)
        : m_blockSize{ 0 }, m_name{ name }, m_vars{}
    {
        for (const auto& [element_name, bytesize] : elements) {
            m_vars[element_name] = m_blockSize;
            m_blockSize += bytesize;
        }
    }

    BlockLayout::operator std::string() const
    {
        return m_name;
    }

    UniformBuffer::UniformBuffer(const char* name, const BlockLayout& layout, const GLenum drawMode)
        : m_name(name)
        , m_blockLayout(layout)
    { 
        glGenBuffers(1, &m_id);
        bind();
        glBufferData(GL_UNIFORM_BUFFER, m_blockLayout.size(), nullptr, drawMode);
        unbind();
    }

}