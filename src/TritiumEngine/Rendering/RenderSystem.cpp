#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Entities/Entity.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  RenderSystem::RenderSystem(const std::shared_ptr<ShaderManager> &shaderManager)
      : m_shaderManager(shaderManager) {}

  void RenderSystem::Draw(const Camera &camera, const World &world) const {
    for (size_t i = 0; i < world.GetNumEntities(); ++i) {
      const Entity *entity = world.GetEntities()[i];

      for (size_t i = 0; i < entity->renderables.size(); ++i) {
        const auto &material   = entity->materials[i];
        const auto &renderable = entity->renderables[i];

        // Apply properties to shader
        m_shaderManager->Use(material->shaderId);
        m_shaderManager->SetMatrix4("model", entity->transform.GetModelMatrix());
        m_shaderManager->SetMatrix4("projectionView", camera.GetProjectionViewMatrix());
        m_shaderManager->SetVector4("color", material->color);

        // Bind VAO and draw the entity to screen
        glBindVertexArray(renderable->GetVAO());
        GLsizei nIndices  = renderable->GetNumIndices();
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