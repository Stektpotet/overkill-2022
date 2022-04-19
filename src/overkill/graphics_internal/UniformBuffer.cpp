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

    UniformBuffer::UniformBuffer(const BlockLayout& layout, const GLenum drawMode)
        : m_name(layout.get_name())
        , m_blockLayout(layout)
    { 
        glGenBuffers(1, &m_id);
        bind();
        glBufferData(GL_UNIFORM_BUFFER, m_blockLayout.size(), nullptr, drawMode);
        unbind();
    }

}
