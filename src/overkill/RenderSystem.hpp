#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "overkill/graphics_internal/VertexArray.hpp"
#include "overkill/graphics_internal/VertexBuffer.hpp"
#include "overkill/graphics_internal/IndexBuffer.hpp"
#include "overkill/graphics_internal/ShaderProgram.hpp"

namespace OK
{
    
struct Rendered
{
    VertexBuffer vbo;
    IndexBuffer<std::uint16_t> ibo;
};


class RenderGroup 
{
    VertexArray vao;
    ShaderProgram program;

    std::vector<std::shared_ptr<Rendered>> rendered_items;

public:
    //void add(Rendered item);

    void bind() const
    {
        vao.bind();
        program.bind();
    }
};

class RenderSystem
{
public:
    std::vector<std::unique_ptr<RenderGroup>> renderQueue;
};

}
