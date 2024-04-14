#pragma once

#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Components/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/Systems/RenderSystem.hpp>

namespace RenderingBenchmark::Systems
{
  template <uint32_t CameraTag> class CubeRenderSystem : public RenderSystem<CameraTag> {
  public:
    CubeRenderSystem() : RenderSystem<CameraTag>({}) {}

    void draw(const Camera &camera) const override {
      auto &registry           = RenderSystem<CameraTag>::m_app->registry;
      auto &shaderManager      = RenderSystem<CameraTag>::m_app->shaderManager;

      registry.view<InstancedRenderable, Shader>().each(
          [&](auto entity, InstancedRenderable &renderable, Shader &shader) {
            unsigned int vao        = renderable.getVao();
            int vertexStride        = renderable.getVertexStride();
            int nVertices           = renderable.getNumVertices();
            int nInstances          = renderable.getNumInstances();
            unsigned int renderMode = renderable.getRenderMode();

            if (shader.id != shaderManager.getCurrentShader()) {
              shaderManager.use(shader.id);
              shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
            }

            // Draw the renderable
            glBindVertexArray(vao);
            glDrawArraysInstanced(renderMode, 0, nVertices / vertexStride, nInstances);
          });
      shaderManager.use(0);
    }
  };
} // namespace RenderingBenchmark::Systems