#pragma once

#include "MeshUtility.hpp"

namespace OK
{
    Mesh make_cube(const glm::vec3 scale, const glm::vec3 relative_position, const bool invert)
    {
        auto indices = std::vector<std::uint16_t> {
            0,  1,  2,      2,  3,  0,
            4,  5,  6,      6,  7,  4,
            8,  9,  10,     10, 11, 8,
            12, 13, 14,     14, 15, 12,
            16, 17, 18,     18, 19, 16,
            20, 21, 22,     22, 23, 20,
        };

        auto vertices = std::vector<glm::vec3> {
			{ -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
			{ -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
		};

		auto uvs = std::vector<glm::vec2> {
			 { 0.0f,  0.0f, },
			 { 1.0f,  0.0f, },
			 { 1.0f,  1.0f, },
			 { 0.0f,  1.0f, },

             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },

             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
			 
			 { 0.0f,  0.0f, },
			 { 1.0f,  0.0f, },
			 { 1.0f,  1.0f, },
			 { 0.0f,  1.0f, },
			 
             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
			 
             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
		};

        Mesh mesh = Mesh{ indices, vertices };
        mesh.uv_coords = uvs;

        regenerate_tbn(mesh);

        return mesh;
    }
    void regenerate_tbn(Mesh& mesh)
    {
        mesh.normals.clear();
        mesh.tangents.clear();
        mesh.bitangents.clear();
        mesh.normals.resize(mesh.vertices.size());
        mesh.tangents.resize(mesh.vertices.size());
        mesh.bitangents.resize(mesh.vertices.size());

        for (size_t i = 0; i < mesh.indices.size(); i += 3)
        {
            std::uint16_t a = mesh.indices[i];
            std::uint16_t b = mesh.indices[i + 1];
            std::uint16_t c = mesh.indices[i + 2];

            /*
            
                 (b)
              d1 /
               (a)--(c)
                  d2
            */

            glm::vec3 d1 = mesh.vertices[b] - mesh.vertices[a];
            glm::vec3 d2 = mesh.vertices[c] - mesh.vertices[a];

            glm::vec2 dUV1 = mesh.uv_coords[b] - mesh.uv_coords[a];
            glm::vec2 dUV2 = mesh.uv_coords[c] - mesh.uv_coords[a];

            float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
            glm::vec3 tangent = (d1 * dUV2.y - d2 * dUV1.y) * r;
            glm::vec3 bitangent = (d2 * dUV1.x - d1 * dUV2.x) * r;
            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent)); // todo: verify direction

            mesh.normals[a] = normal;
            mesh.normals[b] = normal;
            mesh.normals[c] = normal;
            mesh.tangents[a] = tangent;
            mesh.tangents[b] = tangent;
            mesh.tangents[c] = tangent;
            mesh.bitangents[a] = bitangent;
            mesh.bitangents[b] = bitangent;
            mesh.bitangents[c] = bitangent;
        }
    }
}
