#pragma once

#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Components/Renderable.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/Systems/RenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  template <uint32_t CameraTag> class StandardRenderSystem : public RenderSystem<CameraTag> {
  public:
    StandardRenderSystem(BlendOptions blendOptions = {}) : RenderSystem<CameraTag>(blendOptions) {}

    void draw(const Camera &camera) const override {
      bool newDrawCycleStarted = true;
      auto &shaderManager      = RenderSystem<CameraTag>::m_app->shaderManager;
      auto &registry           = RenderSystem<CameraTag>::m_app->registry;

      registry.view<Renderable, Transform, Shader, Color>().each(
          [&](auto entity, Renderable &renderable, Transform &transform, Shader &shader,
              Color &color) {
            // Apply properties to shader
            if (shader.id != shaderManager.getCurrentShader() || newDrawCycleStarted) {
              newDrawCycleStarted = false;
              shaderManager.use(shader.id);
              shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
            }
            shaderManager.setMatrix4("model", transform.getModelMatrix());
            shaderManager.setVector4("color", ColorUtils::ToNormalizedVec4(color));

            unsigned int vao        = renderable.getVao();
            int vertexStride        = renderable.getVertexStride();
            int nVertices           = renderable.getNumVertices();
            int nIndices            = renderable.getNumIndices();
            unsigned int renderMode = renderable.getRenderMode();

            // Draw the renderable
            glBindVertexArray(vao);
            if (nIndices > 0)
              glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
            else
              glDrawArrays(renderMode, 0, nVertices / vertexStride);
          });
    }
  };
} // namespace TritiumEngine::Rendering