#pragma once

namespace OK
{
    class Scene;
    class GameObject;

    struct Component
    {
        virtual void update(float delta_time) {};

    protected:
        virtual void on_created(GameObject& go) {};
    };

    struct GraphicsComponent : public Component
    {
        virtual void render() = 0;
    };

}
