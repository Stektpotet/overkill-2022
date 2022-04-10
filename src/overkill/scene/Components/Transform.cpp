#include "Transform.hpp"

namespace OK
{
    //void Transform::on_created(GameObject* go)
    //{
    //    game_object = go;
    //    orphan();
    //}
    void Transform::orphan()
    {
        set_parent(game_object->get_scene()->root());
    }

    void Transform::on_created(GameObject* go)
    {
        orphan();
    }

    void Transform::set_parent(Transform* transform)
    {
        if (parent != nullptr) {
            for (auto iter = parent->children.begin(); iter != parent->children.end(); ++iter)
            {
                if (*iter == this)
                {
                    parent->children.erase(iter);
                    break;
                }
            }
        }
        parent = transform;
        parent->children.push_back(this);
    }
}
