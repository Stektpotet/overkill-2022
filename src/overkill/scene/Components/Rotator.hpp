#pragma once

#include "BaseComponents.hpp"
#include "Transform.hpp"

namespace OK
{
    struct Rotator : public Component
    {
        void update(float dt) override 
        {
            transform()->rotation = transform()->rotation + glm::vec3{ dt * 900, dt * 9000, 0 };
        }
    };
}
