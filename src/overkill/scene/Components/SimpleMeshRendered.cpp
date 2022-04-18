#include "SimpleMeshRendered.hpp"

#include "Transform.hpp"
#include <overkill/scene/GameObject.hpp>

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

        const auto& transform = game_object->get_transform();
        const auto& trs = transform->get_trs();
        glm::mat3 nrm = glm::transpose(glm::inverse(glm::mat3(trs)));

        for (const auto& renderable : model->renderables)
        {
            renderable.ibo.bind();
            renderable.shader_program->bind();
            glUniformMatrix4fv(renderable.shader_program->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(trs));
            glUniformMatrix3fv(renderable.shader_program->getUniformLocation("NRM"), 1, GL_FALSE, glm::value_ptr(nrm));
            glDrawElements(GL_TRIANGLES, renderable.ibo.count, renderable.ibo.index_type, nullptr);
        }
    }
}
