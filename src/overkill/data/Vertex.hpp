#pragma once
#include <glm/glm.hpp>
#include <overkill/graphics_internal/VertexLayout.hpp>

namespace OK
{
    struct ViewportVertex
    {
        glm::vec2  position = glm::vec2(0, 0);
        static const InterleavingVertexLayout get_vertex_layout();
    };

    struct FullVertex
    {
        glm::vec3  position = glm::vec3(0, 0, 0);
        glm::vec3  normal = glm::vec3(0, 1, 0);
        glm::vec2  uv = glm::vec2(0, 0);
        glm::vec3  tangent = glm::vec3(1, 0, 0);
        glm::vec3  bitangent = glm::vec3(0, 0, -1);
        glm::u8vec4 color = glm::u8vec4(255, 255, 255, 255);

        static const InterleavingVertexLayout get_vertex_layout();
    };

}
