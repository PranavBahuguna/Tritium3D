#include <TritiumEngine/Rendering/RenderSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Rendering
{
  void RenderSystem::Draw(const Camera &camera, const World &world) const {
    for (size_t i = 0; i < world.GetNumEntities(); ++i) {
      const Entity *entity = world.GetEntities()[i];

      for (size_t i = 0; i < entity->renderables.size(); ++i) {
        const auto &material = entity->materials[i];
        const auto &renderable = entity->renderables[i];
        const auto &shader = material->shader;

        // Apply properties to shader
        glUseProgram(shader->GetID());
        shader->SetMatrix4("model", entity->transform.GetModelMatrix());
        shader->SetMatrix4("projectionView", camera.GetProjectionViewMatrix());
        shader->SetVector4("color", material->color);

        // Bind VAO and draw the entity to screen
        glBindVertexArray(renderable->GetVAO());
        GLsizei nIndices = renderable->GetNumIndices();
        GLenum renderMode = renderable->GetRenderMode();

        if (nIndices > 0)
          glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
        else
          glDrawArrays(renderMode, 0, renderable->GetNumVertices());
      }
    }
  }

  void RenderSystem::DrawInstanced(const Camera &camera, const World &world) const {}
} // namespace TritiumEngine::Rendering