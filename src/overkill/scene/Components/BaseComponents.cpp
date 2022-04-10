#include "BaseComponents.hpp"
#include "../GameObject.hpp"
namespace OK
{
    //Component(GameObject* owner);

    void Component::on_created(GameObject* go)
    {
        game_object = go;
        std::cout << go->get_name() << std::endl;
    }

    //void Component::on_created(GameObject* go) { game_object = go; }
    /*  Component::Component(GameObject* owner)
        : game_object{ owner }
    {}

    GraphicsComponent::GraphicsComponent(GameObject* owner) : Component(owner)
    {}*/
}
