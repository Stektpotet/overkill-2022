#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include <overkill/data/Mesh.hpp>
#include <overkill/graphics_internal/Texture.hpp>

namespace OK
{
    Mesh<> make_terrain(RawTexture16 heightmap, const glm::vec3 scale = glm::vec3{ 1 }, const glm::vec3 relative_position = glm::vec3{ 0 });
    Mesh<> make_cube(const glm::vec3 scale = glm::vec3{ 1 }, const glm::vec3 relative_position = glm::vec3{ 0 }, const bool invert = false);

    void regenerate_tbn(
        std::vector<std::uint32_t>& indices,
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec2>& uv,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec3>& tangents,
        std::vector<glm::vec3>& bitangents
    );

}
