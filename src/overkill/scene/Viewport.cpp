#include "Viewport.hpp"
#include <overkill/ShaderSystem.hpp>
#include <overkill/TextureSystem.hpp>

namespace OK
{
    Viewport::Viewport() : vao(), viewport_shader { ShaderSystem::get(ShaderSystem::VIEWPORT_DEFAULT) }
    {
        // Clipped Triangle covering viewport
        std::vector<ViewportVertex> viewport_vertices = { // A perfect triangle covering the viewport accross [-1,1]
            ViewportVertex{ { -3, -1 } },
            ViewportVertex{ {  1, -1 } },
            ViewportVertex{ {  1,  3 } }
        };
        vbo = VertexBuffer(viewport_vertices.size() * sizeof(ViewportVertex), viewport_vertices.data());
        ViewportVertex::get_vertex_layout().applyToBuffer(vbo);
    }

    void Viewport::render()
    {
        vao.bind();
        vbo.bind();
        viewport_shader->bind();
        TextureSystem::get2D("__RTEX_COLOR__").bind(0);
        TextureSystem::get2D("__RTEX_DEPTH__").bind(1);
        glUniform1i(viewport_shader->getUniformLocation("_color"), 0);
        glUniform1i(viewport_shader->getUniformLocation("_depth"), 1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
