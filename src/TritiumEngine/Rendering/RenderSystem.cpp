#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Color.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <entt/entity/registry.hpp>

namespace TritiumEngine::Rendering
{
  void RenderSystem::update(float dt) {
    const Camera &camera = m_app->registry.get<Camera>(m_app->sceneManager.getCurrentSceneEntity());
    draw(camera);
    drawInstanced(camera);
  }

  void RenderSystem::draw(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<Renderable, Transform, Shader, Color>().each(
        [&](auto entity, Renderable &renderable, Transform &transform, Shader &shader,
            Color &color) {
          // Apply properties to shader
          shaderManager.Use(shader.id);
          shaderManager.setMatrix4("model", transform.getModelMatrix());
          shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
          shaderManager.setVector4("color", color.asNormalizedVec4());

          GLuint vao        = renderable.getVao();
          GLsizei nIndices  = renderable.getNumIndices();
          GLsizei nVertices = renderable.getNumVertices();
          GLenum renderMode = renderable.getRenderMode();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
          else
            glDrawArrays(renderMode, 0, nVertices);
        });
  }

  void RenderSystem::drawInstanced(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<InstancedRenderable, Shader>().each(
        [&](auto entity, InstancedRenderable &renderable, Shader &shader) {
          GLuint vao          = renderable.getVao();
          GLsizei nIndices    = renderable.getNumIndices();
          GLsizei nVertices   = renderable.getNumVertices();
          GLsizei nInstances  = renderable.getNumInstances();
          GLenum renderMode   = renderable.getRenderMode();
          uint32_t instanceId = renderable.getInstanceId();

          shaderManager.Use(shader.id);
          shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());

          // Update model matrices
          GLsizei index      = 0;
          auto instancesView = m_app->registry.view<Transform, InstancedRenderableTag>();
          m_app->registry.view<Transform, Color, InstancedRenderableTag>().each(
              [&](auto entity, Transform &transform, Color &color, InstancedRenderableTag &tag) {
                if (index < nInstances && tag.value == instanceId)
                  renderable.setInstanceData(index++, {transform.getModelMatrix(), color.value});
              });
          renderable.updateInstanceDataBuffer();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElementsInstanced(renderMode, nIndices, GL_UNSIGNED_INT, 0, nInstances);
          else
            glDrawArraysInstanced(renderMode, 0, nVertices, nInstances);
        });
  }
} // namespace TritiumEngine::Rendering