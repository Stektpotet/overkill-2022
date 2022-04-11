#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

namespace OK
{
    struct Mesh
    {
        std::vector<std::uint16_t> indices;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::vector<glm::vec2> uv_coords;
        std::vector<glm::u8vec4> color;
    };


    struct MeshPacked // TODO: make use of packed variant
    {
        std::vector<std::uint16_t> indices;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv_coords;
        std::vector<glm::u8vec4> color;
    };

    Mesh make_cube(const glm::vec3 scale = glm::vec3{ 1 }, const glm::vec3 relative_position = glm::vec3{ 0 }, const bool invert = false);

    void regenerate_tbn(Mesh& mesh);

}
