#pragma once

#include <glm/glm.hpp>

#include "BaseComponents.hpp"
#include "Transform.hpp"

namespace OK
{
    struct Rotator : public Component
    {
        float time_rate = 500;
        // Set to Gjøvik latitude
        glm::vec3 rotation_axis = glm::vec3(0.0f, glm::sin(glm::radians(60.8f)), glm::cos(glm::radians(60.8f)));

        void update(float dt) override;
    };
}
