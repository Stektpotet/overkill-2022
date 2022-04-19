#pragma once

#include "BaseComponents.hpp"
#include <glm/glm.hpp>

namespace OK
{

struct Camera : public Component
{
    float field_of_view = 90;
    float aspect_ratio = 16.0f/9.0f;
    float near_clip = 0.01f;
    float far_clip = 1000.0f;

    float movement_speed = 0.1f;

    Camera() = default;
    Camera(float fov, float aspect, float near_clip, float far_clip);

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 view_projection_matrix();
    glm::mat4 view_projection_inverse_matrix();

    void move(float dt);
    void late_update(float dt) override;

private:
    float shift_multiplier = 10;
    float pitch, yaw;

};

}

