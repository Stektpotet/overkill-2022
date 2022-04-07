#pragma once

#include "BaseComponents.hpp"

#include <overkill/RenderSystem.hpp>

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
    
    struct MeshPacked // TODO: make packed variant
    {
        std::vector<std::uint16_t> indices;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv_coords;
        std::vector<glm::u8vec4> color;
    };


    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TMesh">Mesh Type, allows for different mesh renderer types for the Rendergroups that may differ in VAO attributes (i.e. mesh structure layout)</typeparam>
    template<typename TMesh>
    class MeshRenderer : public GraphicsComponent
    {
        TMesh mesh;
        Rendered buffers;


        // Inherited via GraphicsComponent
        virtual void render() override
        {
            buffers.vbo.bind();
            buffers.ibo.bind();

            glDrawElements(GL_TRIANGLES, buffers.ibo.count, buffers.ibo.index_type, nullptr);
        }

    };


}
