#pragma once
#include <overkill/graphics_internal/VertexArray.hpp>
#include <overkill/graphics_internal/VertexBuffer.hpp>
#include <overkill/graphics_internal/ShaderProgram.hpp>
#include <overkill/data/Vertex.hpp>

namespace OK
{
    class Viewport
    {

        VertexArray vao;
        VertexBuffer vbo;

        ShaderProgram* viewport_shader;

    public:
        Viewport();
        void render();
    };
}
