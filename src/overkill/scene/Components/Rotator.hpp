#pragma once

#include <glm/glm.hpp>

#include "BaseComponents.hpp"
#include "Transform.hpp"

namespace OK
{
    struct Rotator : public Component
    {
        bool active = false;
        float time_rate = 0.1;
        glm::vec3 rotation_axis = glm::vec3(0.0f, glm::sin(glm::radians(63.4f)), glm::cos(glm::radians(63.4f)));
        void on_created() override;
        void update(float dt) override;
    };
}
