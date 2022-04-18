#include "Vertex.hpp"

namespace OK
{
    const InterleavingVertexLayout ViewportVertex::get_vertex_layout()
    {
        return { {"position", sizeof(glm::vec2), 2, GL_FLOAT} };
    }

    const InterleavingVertexLayout FullVertex::get_vertex_layout()
    {
        return {
            {"position", sizeof(glm::vec3), 3, GL_FLOAT},                   // 12: 12
            {"normal", sizeof(glm::vec3), 3, GL_FLOAT},                     // 12: 24
            {"uv", sizeof(glm::vec2), 2, GL_FLOAT},                         // 8:  32
            {"tangent", sizeof(glm::vec3), 3, GL_FLOAT},                    // 12: 44
            {"bitangent", sizeof(glm::vec3), 3, GL_FLOAT},                  // 12  56
            {"color", sizeof(glm::u8vec4), 4, GL_UNSIGNED_BYTE, GL_TRUE},   // 4   60 TODO: allocate 4 more bytes for optimal performance
        };
    }
}
