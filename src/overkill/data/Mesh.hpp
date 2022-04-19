#pragma once
#include <glm/glm.hpp>
#include <overkill/graphics_internal/VertexLayout.hpp>
#include "Vertex.hpp"

namespace OK
{
    template<typename TVertex = FullVertex, typename TIndex = std::uint32_t>
    struct Mesh
    {
        const InterleavingVertexLayout vertex_layout = TVertex::get_vertex_layout();
        std::vector<TIndex> indices;
        std::vector<TVertex> vertices;
        size_t size() {
            return sizeof(TVertex) * vertices.size();
        }
    };
}
