#include "CloudController.hpp"
#include <overkill/graphics_internal/UniformBuffer.hpp>

namespace OK
{
    CloudController::CloudController(UniformBuffer* clouds, glm::vec3 min, glm::vec3 max) 
        : Component(),
        cloud_buffer{clouds}, bounds_min{min}, bounds_max{max}
    {
    }
    void CloudController::on_created()
    {
        if (!cloud_buffer)
            cloud_buffer = ShaderSystem::get_uniform_buffer("OK_Clouds");

        auto packed = pack_data(
            glm::vec4(bounds_min, 1),
            glm::vec4(bounds_max, 1)
        );
        cloud_buffer->update(sizeof(packed), &packed);
    }
}
