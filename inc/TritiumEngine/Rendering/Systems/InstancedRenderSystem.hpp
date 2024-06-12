#pragma once

#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Components/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/Systems/RenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

namespace TritiumEngine::Rendering
{
  template <uint32_t CameraTag> class InstancedRenderSystem : public RenderSystem<CameraTag> {
  public:
    InstancedRenderSystem(RenderSettings renderOptions = {})
        : RenderSystem<CameraTag>(renderOptions) {}

    void draw(const Camera &camera) const override {
      auto &shaderManager = RenderSystem<CameraTag>::m_app->shaderManager;
      auto &registry      = RenderSystem<CameraTag>::m_app->registry;

      registry.view<InstancedRenderable, Shader>().each(
          [&](auto entity, InstancedRenderable &renderable, Shader &shader) {
            unsigned int vao        = renderable.getVao();
            int vertexStride        = renderable.getVertexStride();
            int nVertices           = renderable.getNumVertices();
            int nIndices            = renderable.getNumIndices();
            int nInstances          = renderable.getNumInstances();
            unsigned int renderMode = renderable.getRenderMode();
            uint32_t instanceId     = renderable.getInstanceId();

            if (shader.id != shaderManager.getCurrentShader()) {
              shaderManager.use(shader.id);
              shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
            }

            // Update model matrices and color
            int index = 0;
            registry.view<Transform, Color, InstanceTag>().each(
                [&](auto entity, Transform &transform, Color &color, InstanceTag &tag) {
                  if (tag.value == instanceId)
                    renderable.setInstanceData(index++, {transform.getModelMatrix(), color.value});
                });
            renderable.updateInstanceDataBuffer();

            // Draw the renderable
            glBindVertexArray(vao);
            if (nIndices > 0)
              glDrawElementsInstanced(renderMode, nIndices, GL_UNSIGNED_INT, 0, nInstances);
            else
              glDrawArraysInstanced(renderMode, 0, nVertices / vertexStride, nInstances);
          });
      shaderManager.use(0);
    }
  };
} // namespace TritiumEngine::Rendering