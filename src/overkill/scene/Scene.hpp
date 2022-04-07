#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "Components/Transform.hpp"

namespace OK
{
    struct Transform;

    class Scene : std::enable_shared_from_this<Scene>
    {
        friend class GameObject;
        std::unique_ptr<Transform> scene_graph;
        std::vector<std::shared_ptr<GameObject>> objects;

    
    public:
        std::shared_ptr<Scene> get_ptr() { return shared_from_this(); }

        // No public constructor, only a factory function,
        // so there's no way to have getptr return nullptr.
        [[nodiscard]] static std::shared_ptr<Scene> create_scene() {
            // Not using std::make_shared<Scene> because the c'tor is private.
            return std::shared_ptr<Scene>(new Scene());
        }

        const std::unique_ptr<Transform>& root() const;
        const std::shared_ptr<GameObject>& add_game_object(const char* name);
        void propagate_scene_graph();

    private:
        Scene();
        void propagate_trs(Transform* transform, glm::mat4 parent_trs);
    };

}
