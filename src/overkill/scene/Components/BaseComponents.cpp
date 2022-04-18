#include "BaseComponents.hpp"
#include "../GameObject.hpp"
#include "Transform.hpp"

namespace OK
{
    const std::vector<std::type_index> Component::component_type_signatures() const
    {
        return { std::type_index(typeid(*this)) };
    }
    Transform* const& Component::transform() const
    {
        return game_object->get_transform();
    }


    void Component::update(float delta_time) {}

    void Component::late_update(float delta_time) {}

    void Component::on_created() {}

    void Component::on_start_game() {}
}
