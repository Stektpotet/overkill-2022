#include "Scene.hpp"

namespace OK
{
    Scene::Scene(const char* name) 
        : GameObject(name, this),
        objects{}
    {
        scene_graph = add_component<Transform>();
    }
    Transform* const & Scene::root() const
    {
        return scene_graph;
    }

    GameObject*& Scene::add_game_object(const char* name)
    {
        objects.emplace_back(new GameObject(name, this)); 
        return objects.back();
    }

    void Scene::propagate_scene_graph()
    {
        propagate_trs(scene_graph, glm::mat4(1.0f));
    }
    
    void Scene::propagate_trs(Transform* transform, glm::mat4 parent_trs)
    {
        transform->trs = parent_trs * (
            glm::translate(transform->position) * 
            glm::rotate(transform->rotation.y, glm::vec3(0, 1, 0)) * 
            glm::rotate(transform->rotation.x, glm::vec3(1, 0, 0)) * 
            glm::rotate(transform->rotation.z, glm::vec3(0, 0, 1)) * 
            glm::scale(transform->scale)
        );
        for (auto& child : transform->children)
        {
            propagate_trs(child, transform->trs);
        }
    }
}
