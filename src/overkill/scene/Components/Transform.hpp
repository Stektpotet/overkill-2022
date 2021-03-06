#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <memory>

#include "BaseComponents.hpp"
//#include "../GameObject.hpp"
//#include "../Scene.hpp"

namespace OK
{
    class Scene;
    class GameObject;

    struct Transform : public Component
    {
        //[[nodiscard]] static std::shared_ptr<Transform> create_shared() {
        //    return std::shared_ptr<Transform>(new Transform());
        //}
        //[[nodiscard]] static std::unique_ptr<Transform> create_unique() {
        //    return std::unique_ptr<Transform>(new Transform());
        //}

        glm::vec3 position = { 0, 0, 0 };
        glm::quat rotation = glm::quat(glm::vec3{ 0,0,0 });
        glm::vec3 scale = { 1, 1, 1 };

        //Transform(GameObject* owner);

        virtual void on_created() override;

        void set_parent(Transform* transform);
        void orphan();

        
        const glm::mat4 get_trs() const;
        const glm::mat4 local_trs() const;

        inline glm::vec3 forward() const
        {
            return rotation * glm::vec3{ 0, 0, 1 };
        }
        inline glm::vec3 up() const
        {
            return rotation * glm::vec3{ 0, 1, 0 };
        }
        inline glm::vec3 right() const
        {
            return rotation * glm::vec3{ 1, 0, 0 };
        }

    private:
        friend class Scene;


        glm::mat4 trs = glm::mat4(1.0f);
        Transform* parent = nullptr;
        std::vector<Transform*> children;
    };

}
