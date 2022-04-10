#include "GameObject.hpp"
#include <iostream>
namespace OK
{
    GameObject::GameObject(const char* name, Scene* scene)
        : name{ name }
        , scene { scene }
        , components{}
    {
    }
    const Scene* const GameObject::get_scene() const
    {
        return scene;
    }
    GameObject::~GameObject()
    {
        std::cout << "Destroying GameObject: '" << name << "'..." << std::endl;
        for (auto comp : components) {
            delete comp;
        }
        components.clear();
    }
}
