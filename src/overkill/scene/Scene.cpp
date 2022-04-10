#include "Scene.hpp"

namespace OK
{
    Scene::Scene(const char* name) 
        : GameObject(name, this),
        objects{}
    {
        // We cannot use add_component here, as it will attempt to put the parent of the transform to this unconstructed scene's transform.
        scene_graph = new Transform();
        scene_graph->game_object = this;
        components.push_back(scene_graph);
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

    Scene::~Scene()
    {
        for (auto game_object : objects) {
            delete game_object;
        }
        objects.clear();
        scene_graph = nullptr; // pointee deleted by superclass Destructor through deletion of components

        /*
        * IN ~GameObject

        for (auto comp : components) { // deletes scene_graph
            delete comp;
        }
        components.clear();

        */
    }
}
