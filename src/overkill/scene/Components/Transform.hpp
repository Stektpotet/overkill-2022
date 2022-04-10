#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "BaseComponents.hpp"
#include "../GameObject.hpp"
#include "../Scene.hpp"

namespace OK
{

    struct Transform : public Component
    {
        //[[nodiscard]] static std::shared_ptr<Transform> create_shared() {
        //    return std::shared_ptr<Transform>(new Transform());
        //}
        //[[nodiscard]] static std::unique_ptr<Transform> create_unique() {
        //    return std::unique_ptr<Transform>(new Transform());
        //}

        glm::vec3 position = { 0, 0, 0 };
        glm::vec3 rotation = { 0, 0, 0 };
        glm::vec3 scale = { 1, 1, 1 };

        //Transform(GameObject* owner);

        //void on_created(GameObject* go) override;
        void set_parent(Transform* transform);
        void orphan();

    private:
        friend class Scene;


        glm::mat4 trs = glm::mat4(1.0f);
        Transform* parent = nullptr;
        std::vector<Transform*> children;
    };

}
