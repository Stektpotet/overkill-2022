#include "Terrain.hpp"
#include "Transform.hpp"
#include <overkill/scene/GameObject.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OK
{
    Terrain::Terrain(RawTexture16 heightmap)
    {
        Mesh<OK::FullVertex, std::uint32_t> terrain_mesh;

        auto& vertices = terrain_mesh.vertices;
        vertices.resize(heightmap.width * heightmap.height);
        terrain_mesh.indices.reserve(heightmap.width * heightmap.height * 6);

        auto appendVertex = [&vertices, heightmap](int index, int x, int y) {
            constexpr const int SHORT_MAX = 65536;
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
                { xNormalized, pixelHeight, yNormalized },          //position
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
        terrain_model.renderables.emplace_back(terrain_mesh);
        terrain_model.vbo = OK::VertexBuffer(terrain_mesh.size(), terrain_mesh.vertices.data());
        terrain_mesh.vertex_layout.applyToBuffer(terrain_model.vbo);
    }
    void Terrain::on_created()
    {

    }

    void Terrain::render()
    {
        terrain_model.vao.bind();

        const auto& transform = game_object->get_transform();
        const auto& trs = transform->get_trs();

        for (const auto& renderable : terrain_model.renderables)
        {
            renderable.ibo.bind();
            renderable.shader_program->bind();

            glUniformMatrix4fv(renderable.shader_program->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(trs));
            glDrawElements(GL_TRIANGLES, renderable.ibo.count, renderable.ibo.index_type, nullptr);
        }
    }

}
