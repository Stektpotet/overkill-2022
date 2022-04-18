#include "Transform.hpp"
#include "../GameObject.hpp"
#include "../Scene.hpp"
#include <glm/gtx/transform.hpp>

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

    const glm::mat4 Transform::get_trs() const
    {
        return trs;
    }

    const glm::mat4 Transform::local_trs() const
    {
        return (
            glm::translate(position) *
            glm::rotate(rotation.y, glm::vec3(0, 1, 0)) *
            glm::rotate(rotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(rotation.z, glm::vec3(0, 0, 1)) *
            glm::scale(scale)
        );
    }

    void Transform::on_created()
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
