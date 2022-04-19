#pragma once

#include "BaseComponents.hpp"

#include <overkill/ShaderSystem.hpp>
#include <glm/glm.hpp>

namespace OK
{
    struct CloudController : public Component
    {
        CloudController(UniformBuffer* clouds, glm::vec3 min, glm::vec3 max);

        UniformBuffer* cloud_buffer;
        glm::vec3 bounds_min, bounds_max;

        void on_created() override;

    };

}
