#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  void RenderSystem::draw(const Camera &camera) const {
    if (m_drawMode == DrawMode::STANDARD)
      drawStandard(camera);
    else
      drawInstanced(camera);
  }

  void RenderSystem::update(const float dt) {
    const Camera &camera = m_app->registry.get<Camera>(m_app->getCurrentSceneEntity());
    draw(camera);
  }

  void RenderSystem::setDrawMode(DrawMode drawMode) { m_drawMode = drawMode; }

  void RenderSystem::drawStandard(const Camera &camera) const {
    m_app->registry.view<Renderable, Material, Transform>().each(
        [&](auto entity, Renderable &renderable, Material &material, Transform &transform) {
          ShaderManager &shaderManager = m_app->shaderManager;

          // Apply properties to shader
          shaderManager.Use(material.shaderId);
          shaderManager.setMatrix4("model", transform.getModelMatrix());
          shaderManager.setMatrix4("projectionView", camera.getProjectionViewMatrix());
          shaderManager.setVector4("color", material.color);

          // Bind VAO and draw the entity to screen
          glBindVertexArray(renderable.getVAO());
          GLsizei nIndices  = renderable.getNumIndices();
          GLenum renderMode = renderable.getRenderMode();

          if (nIndices > 0)
            glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
          else
            glDrawArrays(renderMode, 0, renderable.getNumVertices());
        });
  }

  void RenderSystem::drawInstanced(const Camera &camera) const {}
} // namespace TritiumEngine::Rendering