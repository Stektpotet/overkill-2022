#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "overkill/graphics_internal/VertexArray.hpp"
#include "overkill/graphics_internal/VertexBuffer.hpp"
#include "overkill/graphics_internal/IndexBuffer.hpp"
#include "overkill/graphics_internal/ShaderProgram.hpp"

namespace OK
{

class RenderGroup 
{
    VertexArray vao;
    ShaderProgram program;

    std::vector<Rendered> rendered_items;

public:
    void add(Rendered item);

    void bind() const
    {
        vao.bind();
        program.bind();
    }
};

class Rendered 
{
    VertexBuffer vbo;
    IndexBuffer<std::uint16_t> ibo;
    
};


class RenderSystem
{
    std::vector<RenderGroup> renderQueue;
};

}