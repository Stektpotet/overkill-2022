#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "BaseComponents.hpp"

#include <overkill/graphics_internal/IndexBuffer.hpp>
#include <overkill/graphics_internal/VertexArray.hpp>
#include <overkill/graphics_internal/VertexBuffer.hpp>
#include <overkill/graphics_internal/ShaderProgram.hpp>
#include <overkill/ShaderSystem.hpp>

#include <overkill/data/Model.hpp>

namespace OK
{
struct SimpleMeshRenderer : public GraphicsComponent
{
    Model* model;
    virtual void on_created() override;

    // Inherited via GraphicsComponent
    virtual void render() override;
};

}
