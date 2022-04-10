#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Components/BaseComponents.hpp"

namespace OK
{
    struct Component;

    struct GameObject
    {
        friend class Scene;
    private:
        std::string name;
        Scene* scene;

        std::vector<Component*> components;

    public:

        inline const std::string_view get_name() const {
            return name;
        }

        GameObject(const char* name, Scene* scene);

        const Scene* const get_scene() const;

        template<typename TComponent, typename... Args> //, std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
        TComponent* add_component(Args&&... args);

        ~GameObject();
    };

    template<typename TComponent, typename ...Args>
    inline TComponent* GameObject::add_component(Args && ...args)
    {
        TComponent* component = (TComponent*)components.emplace_back(new TComponent(std::forward<Args>(args)...));
        component->game_object = this;
        component->on_created(this);
        return component;
    }
}
