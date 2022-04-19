#include "Rotator.hpp"
#include "Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OK
{
    void Rotator::on_created()
    {
        transform()->rotation = glm::quatLookAt(glm::vec3{ 1,0,0 }, rotation_axis);
    }
    void Rotator::update(float dt)
{
    transform()->rotation = glm::rotate(transform()->rotation, glm::radians(time_rate), rotation_axis);
}
}
