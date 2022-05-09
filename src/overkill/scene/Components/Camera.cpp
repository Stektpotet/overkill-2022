#include "Camera.hpp"
#include <overkill/ShaderSystem.hpp>


#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.hpp"
#include "overkill/scene/Input.hpp"

namespace OK
{
    Camera::Camera(float fov, float aspect, float near_clip, float far_clip)
        : field_of_view{ fov }, aspect_ratio{ aspect },
        near_clip{ near_clip }, far_clip{ far_clip }
    {
    }

    void Camera::move(float dt)
    {
        auto& t = transform();
        
        pitch += Input::m_cursor_deltaY;
        yaw += Input::m_cursor_deltaX;
        
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // HACKHACKHACK
        Input::m_cursor_deltaX = 0;
        Input::m_cursor_deltaY = 0;

        glm::vec3 look_dir = { 1,0,0 };
        look_dir = glm::normalize(glm::rotateY(look_dir, glm::radians(-yaw)));
        glm::vec3 u = glm::cross(glm::vec3{ 0,1,0 }, look_dir);
        look_dir = glm::normalize(glm::rotate(look_dir, glm::radians(pitch), u));
        t->rotation = glm::quatLookAt(look_dir, glm::vec3{ 0,1,0 });

        float speed = movement_speed 
            * (Input::m_shift ? shift_multiplier : 1.0f)
            * (Input::m_ctrl ? 1.0f / shift_multiplier : 1.0f);

        if (Input::getKey(GLFW_KEY_W))
        {
            t->position -= t->forward() * speed * dt;
        }
        else if (Input::getKey(GLFW_KEY_S))
        {
            t->position += t->forward() * speed * dt;
        }

        if (Input::getKey(GLFW_KEY_D))
        {
            t->position += t->right() * speed * dt;
        }
        else if (Input::getKey(GLFW_KEY_A))
        {
            t->position -= t->right() * speed * dt;
        }
        
        if (Input::getKey(GLFW_KEY_E))
        {
            t->position += t->up() * speed * dt;
        }
        else if (Input::getKey(GLFW_KEY_Q))
        {
            t->position -= t->up() * speed * dt;
        }

        
    }

    void Camera::late_update(float delta_time)
    {
        projection_matrix = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_clip, far_clip);
        view_matrix = glm::inverse(transform()->get_trs());

        auto& p = transform()->position;
        auto& m = view_matrix[3];
        auto& k = glm::transpose(view_matrix)[3];

    }
}
