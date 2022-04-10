#include "GameObject.hpp"

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
}
