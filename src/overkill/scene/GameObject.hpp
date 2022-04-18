#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Components/BaseComponents.hpp"

namespace OK
{
    struct Component;
    struct Transform;

    struct GameObject
    {
        friend class Scene;
    private:
        std::string name;
        Scene* scene;

        std::vector<Component*> components;
        Transform* transform;

    public:

        inline const std::string_view get_name() const {
            return name;
        }

        GameObject(const char* name, Scene* scene);

        const Scene* const& get_scene() const;
        Transform* const& get_transform() const;

        template<typename TComponent, typename... Args> //, std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
        TComponent* add_component(Args&&... args);

        ~GameObject();
    };

    template<typename TComponent, typename ...Args>
    inline TComponent* GameObject::add_component(Args && ...args)
    {
        // TODO: Move ownership of components to the component registries to have them all placed sequentially in memory
        //          NOTE: this also changes the behaviour of the destruction!
        TComponent* component = (TComponent*)components.emplace_back(new TComponent(std::forward<Args>(args)...));
        component->game_object = this;
        scene->register_component(component);
        component->on_created();
        return component;
    }
}
