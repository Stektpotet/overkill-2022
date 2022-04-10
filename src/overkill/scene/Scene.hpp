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
#include "../RenderSystem.hpp"

namespace OK
{
    struct Transform;

    class Scene
    {
        //friend class GameObject;
        std::string name;
        Transform* scene_graph;
        std::vector<GameObject*> objects;

    public:
        //RenderSystem renderSystem;


        Scene(const char* name);


        Transform* const & root() const;
        GameObject *& add_game_object(const char* name);
        void propagate_scene_graph();

    private:
        Scene();
        void propagate_trs(Transform* transform, glm::mat4 parent_trs);

        ~Scene();
    };

}
