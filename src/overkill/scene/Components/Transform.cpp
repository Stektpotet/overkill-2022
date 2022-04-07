#include "Transform.hpp"

namespace OK
{
    void Transform::on_created(GameObject& go)
    {
        set_parent(go.get_scene()->root().get());
    }
    void Transform::set_parent(Transform* transform)
    {
        parent.reset(transform);
        parent->children.push_back(shared_from_this());
    }
}
