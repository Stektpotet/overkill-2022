#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace OK
{
    struct DirectionalLight {
        glm::vec3 intensities;
    };

struct PointLight {

    glm::vec3 intensities;
    float constant;
    float linear;
    float quadratic;

    PointLight() :
        intensities{ glm::vec3{1.0f, 1.0f, 1.0f} },
        constant{ 1.0f },
        // Default terms for a range of 100 units
        linear{ 0.02f },
        quadratic{ 0.001f }
    {}
    PointLight(float radius);

    void setRadius(float radius);
};
struct SpotLight : public PointLight {

    SpotLight() : cutoffAngle{ glm::half_pi<float>() } {}
    SpotLight(float reach, float cutoffAngle);

    float cutoffAngle;
};

}
