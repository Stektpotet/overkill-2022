#pragma once

#include "BaseComponents.hpp"

#include <overkill/RenderSystem.hpp>
#include "..\MeshUtility.hpp"

namespace OK
{
    
    //struct Mesh 
    //{
    //    std::vector<std::uint16_t> indices;
    //    std::vector<glm::vec3> vertices;
    //    std::vector<glm::vec3> normals;
    //    std::vector<glm::vec3> tangents;
    //    std::vector<glm::vec3> bitangents;
    //    std::vector<glm::vec2> uv_coords;
    //    std::vector<glm::u8vec4> color;
    //};



    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TMesh">Mesh Type, allows for different mesh renderer types for the Rendergroups that may differ in VAO attributes (i.e. mesh structure layout)</typeparam>
    template<typename TMesh>
    class MeshRenderer : public GraphicsComponent
    {
        TMesh mesh;
        Rendered* buffers;
        
        virtual void on_start_game() override
        {
           /* buffers = new Rendered{
                VertexBuffer(mesh.vertices.size() )
            }*/
        }

        // Inherited via GraphicsComponent
        virtual void render() override
        {

        }

    };

}
