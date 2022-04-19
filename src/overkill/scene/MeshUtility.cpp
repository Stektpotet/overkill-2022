#pragma once

#include "MeshUtility.hpp"
#include <overkill/data/Mesh.hpp>
#include <iostream>

namespace OK
{
    Mesh<> make_terrain(RawTexture16 heightmap, const glm::vec3 scale, const glm::vec3 relative_position)
    {
        std::cout << "INFO: Generating terrain mesh from heightmap..." << std::endl;
        Mesh<OK::FullVertex, std::uint32_t> terrain_mesh;
        auto& vertices = terrain_mesh.vertices;
        vertices.resize(heightmap.width * heightmap.height);
        terrain_mesh.indices.reserve(heightmap.width * heightmap.height * 6);

        auto appendVertex = [&vertices, heightmap, scale, relative_position](int index, int x, int y) {
            constexpr const int SHORT_MAX = 65536;
            auto intensity = heightmap.pixels[index];

            auto pixelHeight = float(heightmap.pixels[index]) / SHORT_MAX;  // 16bit heightmap

            //REF: https://stackoverflow.com/questions/33736199/calculating-normals-for-a-height-map

            // Relative indices (top, bottom, left, right)
            const auto Ti = (index - heightmap.width >= 0) ? index - heightmap.width : index + heightmap.width;
            const auto Bi = (index + heightmap.width < vertices.size()) ? index + heightmap.width : index - heightmap.width;
            const auto Li = (index - 1 >= 0) ? index - 1 : index + 1;
            const auto Ri = (index + 1 < vertices.size()) ? index + 1 : index - 1;

            const auto T = float(heightmap.pixels[Ti]) / SHORT_MAX;
            const auto B = float(heightmap.pixels[Bi]) / SHORT_MAX;
            const auto L = float(heightmap.pixels[Li]) / SHORT_MAX;
            const auto R = float(heightmap.pixels[Ri]) / SHORT_MAX;

            float xUnit = 1.0f / heightmap.width;
            float yUnit = 1.0f / heightmap.height;

            auto tangent = glm::normalize(glm::vec3(2 * xUnit, R - L, 0));
            auto bitangent = glm::normalize(glm::vec3(0, B - T, 2 * yUnit));
            auto normal = glm::normalize(glm::cross(bitangent, tangent));

            float xNormalized = (float(x) / heightmap.width);
            float yNormalized = (float(y) / heightmap.height);

            vertices[index] = FullVertex{
                relative_position + scale * glm::vec3{ xNormalized, pixelHeight, yNormalized },          //position
                normal,                                             //normal - //TODO - heightDiff = glm::abs(a-b);
                { xNormalized, yNormalized },                       //uv
                tangent,
                bitangent,
                { 255u, 255u, 255u, 255u}
            };
        };

        // Move accross quad by quad
        for (std::uint16_t y = 0u; y < heightmap.height - 1; y++)
        {
            for (std::uint16_t x = 0u; x < heightmap.width - 1; x++)
            {
                std::uint32_t baseIndex = x + y * heightmap.width;

                appendVertex(baseIndex, x, y);  //upper left
                appendVertex(baseIndex + heightmap.width, x, y);  //lower left
                appendVertex(baseIndex + 1, x, y);  //upper right
                appendVertex(baseIndex + heightmap.width + 1, x, y);  //lower right

                terrain_mesh.indices.emplace_back(baseIndex);
                terrain_mesh.indices.emplace_back(baseIndex + heightmap.width);
                terrain_mesh.indices.emplace_back(baseIndex + 1);
                terrain_mesh.indices.emplace_back(baseIndex + 1);
                terrain_mesh.indices.emplace_back(baseIndex + heightmap.width);
                terrain_mesh.indices.emplace_back(baseIndex + heightmap.width + 1);
            }
        }
        return terrain_mesh;
    }


    Mesh<> make_cube(const glm::vec3 scale, const glm::vec3 relative_position, const bool invert)
    {
        auto indices = std::vector<std::uint32_t> {
            0,  1,  2,      2,  3,  0,
            4,  5,  6,      6,  7,  4,
            8,  9,  10,     10, 11, 8,
            12, 13, 14,     14, 15, 12,
            16, 17, 18,     18, 19, 16,
            20, 21, 22,     22, 23, 20,
        };
        if (invert) {
            std::reverse(indices.begin(), indices.end());
        }

        /*
        
            3 - 2
            | / |
            0 - 1

            0 - 1
            | \ |
            3 - 2
        */

        auto vertices = std::vector<glm::vec3> {
			{ -0.5f * scale.x, -0.5f * scale.y, 0.5f * scale.z },  // 0
			{  0.5f * scale.x, -0.5f * scale.y, 0.5f * scale.z },  // 1
			{  0.5f * scale.x,  0.5f * scale.y, 0.5f * scale.z },  // 2
			{ -0.5f * scale.x,  0.5f * scale.y, 0.5f * scale.z },  // 3

            { -0.5f * scale.x,  0.5f * scale.y,  -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  -0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
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

        std::vector<glm::vec3> normals, tangents, bitangents;

        regenerate_tbn(indices, vertices, uvs, normals, tangents, bitangents);

        
        Mesh<> mesh = Mesh<>();
        
        for (size_t i = 0; i < vertices.size(); i++)
        {
            mesh.vertices.emplace_back(FullVertex{
                relative_position + vertices[i], normals[i], uvs[i], tangents[i], bitangents[i], glm::u8vec4(255U,255U,255U,255U)
            });
        }
        mesh.indices = indices;
        return mesh;
    }

    void regenerate_tbn(
        std::vector<std::uint32_t>& indices,
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec2>& uv,
        std::vector<glm::vec3>& normals, 
        std::vector<glm::vec3>& tangents, 
        std::vector<glm::vec3>& bitangents
        )
    {
        normals.clear();
        tangents.clear();
        bitangents.clear();
        normals.resize(vertices.size());
        tangents.resize(vertices.size());
        bitangents.resize(vertices.size());

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            std::uint16_t a = indices[i];
            std::uint16_t b = indices[i + 1];
            std::uint16_t c = indices[i + 2];

            /*
            
                 (b)
              d1 /
               (a)--(c)
                  d2
            */

            glm::vec3 d1 = vertices[b] - vertices[a];
            glm::vec3 d2 = vertices[c] - vertices[a];

            glm::vec2 dUV1 = uv[b] - uv[a];
            glm::vec2 dUV2 = uv[c] - uv[a];

            float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
            glm::vec3 tangent = (d1 * dUV2.y - d2 * dUV1.y) * r;
            glm::vec3 bitangent = (d2 * dUV1.x - d1 * dUV2.x) * r;
            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent)); // todo: verify direction

            normals[a] = normal;
            normals[b] = normal;
            normals[c] = normal;
            tangents[a] = tangent;
            tangents[b] = tangent;
            tangents[c] = tangent;
            bitangents[a] = bitangent;
            bitangents[b] = bitangent;
            bitangents[c] = bitangent;
        }
    }
    
    void regenerate_tbn(
        std::vector<std::uint32_t>& indices,
        std::vector<FullVertex>& vertices
    )
    {
        for (size_t i = 0; i < indices.size(); i += 3)
        {
            std::uint16_t a = indices[i];
            std::uint16_t b = indices[i + 1];
            std::uint16_t c = indices[i + 2];

            /*

                 (b)
              d1 /
               (a)--(c)
                  d2
            */

            glm::vec3 d1 = vertices[b].position - vertices[a].position;
            glm::vec3 d2 = vertices[c].position - vertices[a].position;

            glm::vec2 dUV1 = vertices[b].uv - vertices[a].uv;
            glm::vec2 dUV2 = vertices[c].uv - vertices[a].uv;

            float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
            glm::vec3 tangent = (d1 * dUV2.y - d2 * dUV1.y) * r;
            glm::vec3 bitangent = (d2 * dUV1.x - d1 * dUV2.x) * r;
            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent)); // todo: verify direction

            vertices[a].normal = normal;
            vertices[b].normal = normal;
            vertices[c].normal = normal;
            vertices[a].tangent = tangent;
            vertices[b].tangent = tangent;
            vertices[c].tangent = tangent;
            vertices[a].bitangent = bitangent;
            vertices[b].bitangent = bitangent;
            vertices[c].bitangent = bitangent;
        }
    }
}
