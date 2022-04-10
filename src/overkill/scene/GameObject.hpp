#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Components/BaseComponents.hpp"

namespace OK
{
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

        template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
        T* add_component()
        {
            components.push_back(new T());
            T* component = (T*)components.back();
            component->game_object = this;
            component->on_created(this);
            return component;
        }

        ~GameObject();
    };

}
