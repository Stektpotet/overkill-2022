#pragma once
#include <overkill/graphics_internal/VertexArray.hpp>
#include <overkill/graphics_internal/VertexBuffer.hpp>
#include <overkill/graphics_internal/IndexBuffer.hpp>
#include <overkill/graphics_internal/ShaderProgram.hpp>
#include <overkill/ShaderSystem.hpp>

#include <overkill/data/Mesh.hpp>

namespace OK
{
    struct Renderable // GPU-side representation of a mesh
    {
        IndexBuffer<std::uint32_t>     ibo;
        //std::uint16_t material;
        ShaderProgram* shader_program;
        const char* tag;

        inline Renderable(const Mesh<>& mesh, ShaderProgram* shader = nullptr)
            : ibo(mesh.indices.size(), mesh.indices.data()), shader_program{ shader }
        {
            if (!shader_program)
            {
                shader_program = ShaderSystem::get(ShaderSystem::DEFAULT);
            }
        }
    };


    struct Model
    {
        VertexArray             vao;
        VertexBuffer            vbo;
        std::vector<Renderable> renderables;
        const char* tag;
    };
}
