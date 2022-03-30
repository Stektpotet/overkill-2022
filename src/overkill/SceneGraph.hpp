#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>


namespace OK
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 worldspace = glm::identity<glm::mat4>();
    private:
        std::vector<std::shared_ptr<Transform>> children;
    };


}