#include "GameObject.hpp"
#include "Scene.hpp"

namespace OK
{
    GameObject::GameObject(const char* name, std::shared_ptr<Scene>&& scene)
        : name{ name }
        , scene { scene }
        , components{}
    {
    }
    const std::shared_ptr<const Scene> GameObject::get_scene() const
    {
        return scene;
    }
}
