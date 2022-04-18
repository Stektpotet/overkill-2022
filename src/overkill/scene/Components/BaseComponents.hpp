#pragma once
#include <iostream>
#include <typeindex>
#include <vector>

namespace OK
{
    class Scene;
    class GameObject;
    struct Transform;

    struct Component
    {
    public:
        friend class GameObject;
        GameObject* game_object;
        Transform* const& transform() const;

        virtual const std::vector<std::type_index> component_type_signatures() const;

        virtual void update(float delta_time);

        virtual void late_update(float delta_time);

        virtual void on_created();

        virtual void on_start_game();
    };

    struct GraphicsComponent : public Component
    {
        inline virtual const std::vector<std::type_index> component_type_signatures() const override
        {
            return { std::type_index(typeid(*this)), std::type_index(typeid(GraphicsComponent)) };
        }

        virtual void render() = 0;
    protected:
        inline GraphicsComponent() : Component() {};
    };

}
