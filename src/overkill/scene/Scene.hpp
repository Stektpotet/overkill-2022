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

    class Scene : public GameObject
    {
        //friend class GameObject;
        Transform* scene_graph;
        std::vector<GameObject*> objects;

    
    public:
        Scene(const char* name);

        // No public constructor, only a factory function,
        // so there's no way to have getptr return nullptr.
        //[[nodiscard]] static std::shared_ptr<Scene> create_scene() {
        //    // Not using std::make_shared<Scene> because the c'tor is private.
        //    return std::shared_ptr<Scene>(new Scene());
        //}

        Transform* const & root() const;
        GameObject *& add_game_object(const char* name);
        void propagate_scene_graph();

    private:
        Scene();
        void propagate_trs(Transform* transform, glm::mat4 parent_trs);

        ~Scene();
    };

}
