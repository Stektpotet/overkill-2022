#include "Rotator.hpp"
#include "Transform.hpp"
#include "overkill/scene/Input.hpp"


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
        if (Input::getKey(GLFW_KEY_P))
        {
            active = !active;
        }
        if (Input::getKey(GLFW_KEY_M))
        {
            time_rate += 0.125f;
        }
        if (Input::getKey(GLFW_KEY_N))
        {
            time_rate -= 0.125f;
        }

        if (active) {
            transform()->rotation = glm::rotate(transform()->rotation, glm::radians(time_rate * dt), rotation_axis);
        }

    }
}
