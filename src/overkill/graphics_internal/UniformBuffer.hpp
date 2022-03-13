#pragma once
#include <glad/glad.h>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <string>

namespace OK
{
    /// <summary>
    /// Elegant and simple tuple implementation
    /// https://codereview.stackexchange.com/a/67572
    /// </summary>

    template <typename ...Xs>
    auto pack_data(Xs&& ...xs) {
        return [=](auto&& f) mutable -> decltype(auto) {
            return std::forward<decltype(f)>(f)(&xs...);
        };
    }

    template <std::size_t n, typename = std::make_index_sequence<n>>
    struct get_impl;

    template <std::size_t n, std::size_t ...ignore>
    struct get_impl<n, std::index_sequence<ignore...>> {
        template <typename Nth>
        constexpr decltype(auto) operator()(decltype(ignore, (void const*)0)..., Nth nth, ...) const
        {
            return nth;
        }
    };

    template <std::size_t N, typename Tuple>
    decltype(auto) get(Tuple& tuple) {
        return *tuple(get_impl<N>{});
    }




struct BlockLayout
{
private:
    std::unordered_map<std::string, GLuint>      m_vars;
    //std::unordered_map<std::string, BlockLayout> m_blocks;
    GLuint m_blockSize; //the combined size of this block
    std::string m_name;
public:
    BlockLayout(const char* name = "", std::initializer_list<std::pair<const char*, int>> elements = {});
    inline GLsizei size() const { return m_blockSize; }
    explicit operator std::string() const;
    GLuint indexOfUniform(const std::string& name) const
    {
        auto search = m_vars.find(name); //@TODO discuss usage of at() as it works just as well here
        if (search != m_vars.end())
        {
            return search->second;
        }
        //LOG_WARN("indexOfUniform: \"%s\" cannot be found in buffer!\n has it been added in the layout?", name.c_str());
        return 0;
        //return m_vars.at(name);
    }

    void pushBlock(const BlockLayout& block)
    {
        for (const auto& var : block.m_vars)
        {
            m_vars.emplace(std::make_pair(block.m_name + "." + var.first, m_blockSize + var.second));
        }
        m_blockSize += block.m_blockSize;
    }

    void pushBlockArray(const BlockLayout& block, GLuint count) //allows direct access to the inner-element indices
    {

        /* TESTING THE LOGIC BEHIND THIS:

            auto block0 = BlockLayout("b0")
            block0.push<16>("position");
            block0.push<16>("color");
                // block0.size = 32

            block1.push<16>("somevec");
                // block1.size = 16
            block1.pushBlock(block0);
                // somevec,     0
                // position,    16
                // color,       32
                // block1.size = 48
        */
        for (GLuint i = 0; i < count; i++)
        {
            for (const auto& var : block.m_vars)
            {
                m_vars.emplace(std::make_pair(block.m_name + '[' + std::to_string(i) + "]." + var.first, m_blockSize + var.second));
            }
            m_blockSize += block.m_blockSize;
        }
    }

    void push(const std::string& name, GLuint size)
    {
        m_vars.emplace(std::make_pair(name, m_blockSize));
        m_blockSize += size;
    }
};


class UniformBuffer
{

private:
    GLuint      m_id = 0u;
    std::string m_name = "";
    BlockLayout m_blockLayout;
public:

    UniformBuffer() = default;
    UniformBuffer(const char* name, const BlockLayout& layout, const GLenum drawMode);

    inline explicit operator std::string() const
    {
        return m_name;
    } 
    
    inline std::string name()  const
    {
        return m_name;
    }
    inline GLuint get()  const
    {
        return m_id;
    }

    inline explicit operator GLuint() const
    {
        return m_id;
    }

    inline void clean()
    {
        glDeleteBuffers(1, &m_id);
    }

    inline void bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    }

    inline void unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    inline GLuint getUniformIndex(const std::string& name) const
    {
        return m_blockLayout.indexOfUniform(name);
    }

    inline void update(const int index, GLsizeiptr size, const void* data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferSubData(GL_UNIFORM_BUFFER, index, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
};

}