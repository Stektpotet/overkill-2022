#pragma once
#include <iostream>

namespace OK
{
    class Scene;
    class GameObject;

    struct Component
    {
        friend class GameObject;

        GameObject* game_object;
        virtual void update(float delta_time) {};

        virtual void on_created(GameObject* go);
        virtual void on_start_game();
    };

    struct GraphicsComponent : public Component
    {
        virtual void render() = 0;
        virtual void on_created(GameObject* go) override;
    };

}
