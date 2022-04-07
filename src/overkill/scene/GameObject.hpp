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
        std::shared_ptr<Scene> scene;
        std::vector<std::shared_ptr<Component>> components;
        
        GameObject(const char* name, std::shared_ptr<Scene>&& scene);

    public:

        const std::shared_ptr<const Scene> get_scene() const;

        template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
        const std::shared_ptr<T>& add_component()
        {
            components.push_back(std::make_shared<T>());
            std::shared_ptr<T> component = std::static_pointer_cast<T>(components.back());
            component->on_created(*this);
            return component;
        }
    };

}
