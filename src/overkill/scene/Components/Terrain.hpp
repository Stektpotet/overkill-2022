#pragma once
#include "BaseComponents.hpp"
#include <overkill/data/Mesh.hpp>
#include <overkill/data/Model.hpp>

namespace OK
{
    struct Terrain : public GraphicsComponent
    {
        Model terrain_model;
        Terrain(RawTexture16 heightmap);

        void on_created() override;
        void render() override;
    };
}
