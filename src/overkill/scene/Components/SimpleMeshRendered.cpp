#include "SimpleMeshRendered.hpp"

#include "Transform.hpp"
#include <overkill/scene/GameObject.hpp>
#include <overkill/scene/Scene.hpp>
namespace OK
{
    void SimpleMeshRenderer::on_created()
    {
        if (game_object->get_transform() == nullptr)
            std::cout << game_object->get_name() << " does not have a transform, This needs to be ensured for SimpleMeshRenderer" << std::endl;
    }

    void SimpleMeshRenderer::render()
    {
        if (model == nullptr)
            return;

        model->vao.bind();
        const auto& trs = game_object->get_transform()->get_trs();
        const auto& cam = game_object->get_scene()->active_camera();

        glm::mat3 nrm = glm::transpose(glm::inverse(glm::mat3(trs)));
        glm::mat4 mvp = cam->projection_matrix * cam->view_matrix * trs;

        for (const auto& renderable : model->renderables)
        {
            renderable.ibo.bind();
            renderable.shader_program->bind();

            glUniformMatrix4fv(renderable.shader_program->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
            glUniformMatrix4fv(renderable.shader_program->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(trs));
            glUniformMatrix3fv(renderable.shader_program->getUniformLocation("NRM"), 1, GL_FALSE, glm::value_ptr(nrm));
            glDrawElements(GL_TRIANGLES, renderable.ibo.count, renderable.ibo.index_type, nullptr);
        }
    }
}
