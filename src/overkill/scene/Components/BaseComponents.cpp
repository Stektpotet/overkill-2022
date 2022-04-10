#include "BaseComponents.hpp"
#include "../GameObject.hpp"
#include "../Scene.hpp"
namespace OK
{
    //Component(GameObject* owner);

    void Component::on_created(GameObject* go)
    {
    }

    void GraphicsComponent::on_created(GameObject* go)
    {
        std::cout << go->get_name()<< "::GraphicsComponent" << std::endl;
        //go->get_scene()->renderSystem.renderQueue
    }

    //void Component::on_created(GameObject* go) { game_object = go; }
    /*  Component::Component(GameObject* owner)
        : game_object{ owner }
    {}

    GraphicsComponent::GraphicsComponent(GameObject* owner) : Component(owner)
    {}*/
}
