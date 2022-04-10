#pragma once

namespace OK
{
    class Scene;
    class GameObject;

    struct Component
    {
        friend class GameObject;

        GameObject* game_object;
        virtual void update(float delta_time) {};

        //Component(GameObject* owner);
        //virtual void on_created(GameObject* go);
    };

    struct GraphicsComponent : public Component
    {
        virtual void render() = 0;
        //GraphicsComponent(GameObject* owner);
    };

}