#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "overkill/graphics_internal/VertexArray.hpp"
#include "overkill/graphics_internal/VertexBuffer.hpp"
#include "overkill/graphics_internal/IndexBuffer.hpp"
#include "overkill/graphics_internal/ShaderProgram.hpp"

namespace OK
{
    class Scene;
    
struct Rendered
{
    VertexBuffer* vbos;
    IndexBuffer<std::uint16_t> ibo;
};

class RenderGroup 
{
    VertexArray vao;
    ShaderProgram program;

    std::vector<Rendered*> rendered_items;

public:
    //void add(Rendered item);

    /*void bind() const
    {
        vao.bind();
        program.bind();
    }*/
};

class RenderSystem
{
private:
    //static ComponentRegistry<SimpleMeshRenderer>* meshRenderers; // Owned by scene
    std::vector<RenderGroup> renderQueue;
public:
    void static_batching(OK::Scene& scene);
};

}
