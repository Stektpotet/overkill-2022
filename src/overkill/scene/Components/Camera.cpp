#include "Camera.hpp"
#include <overkill/ShaderSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "Transform.hpp"

namespace OK
{
    Camera::Camera(float fov, float aspect, float near_clip, float far_clip)
        : field_of_view{ fov }, aspect_ratio{ aspect },
        near_clip{ near_clip }, far_clip{ far_clip }
    {
    }

    void Camera::late_update(float delta_time)
    {
        projection_matrix = glm::perspective(field_of_view, aspect_ratio, near_clip, far_clip);
        view_matrix = glm::inverse(transform()->get_trs());
    }
}
