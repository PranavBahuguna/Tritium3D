#pragma once

#include <TritiumEngine/Physics/Components/AABB.hpp>
#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Components/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/Systems/RenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

using namespace TritiumEngine::Physics;

namespace RenderingBenchmark::Systems
{
  template <uint32_t CameraTag> class CirclesRenderSystem : public RenderSystem<CameraTag> {
  public:
    CirclesRenderSystem(RenderSettings renderSettings = {}, int minEntities = 10000,
                        int maxEntities = 200000)
        : RenderSystem<CameraTag>(renderSettings), m_minEntities(minEntities),
          m_maxEntities(maxEntities) {
      // Calculate scale factor
      m_logScaleFactor =
          static_cast<float>(MAX_CIRCLE_SIDES) / logf((float)m_minEntities / m_maxEntities);
      glPointSize(2);
    }

    void draw(const Camera &camera) const override {
      auto &shaderManager = RenderSystem<CameraTag>::m_app->shaderManager;
      auto &registry      = RenderSystem<CameraTag>::m_app->registry;

      // Calculate projection view matrix and world-space camera bounds
      const auto &projViewMatrix = camera.calcProjectionViewMatrix();
      const auto &invProjMatrix  = glm::inverse(projViewMatrix);
      const auto &bottomLeft     = invProjMatrix * glm::vec4(-1.f, -1.f, 0.f, 1.f);
      const auto &topRight       = invProjMatrix * glm::vec4(1.f, 1.f, 0.f, 1.f);
      float left                 = bottomLeft.x;
      float right                = topRight.x;
      float bottom               = bottomLeft.y;
      float top                  = topRight.y;

      registry.view<InstancedRenderable>().each([&](auto entity, InstancedRenderable &renderable) {
        unsigned int vao        = renderable.getVao();
        int vertexStride        = renderable.getVertexStride();
        int nVertices           = renderable.getNumVertices();
        unsigned int renderMode = renderable.getRenderMode();
        uint32_t instanceId     = renderable.getInstanceId();

        // Update model matrices and color
        int index = 0;
        registry.view<Transform, AABB, Color, InstanceTag>().each(
            [&](auto entity, Transform &transform, AABB &aabb, Color &color, InstanceTag &tag) {
              if (tag.value != instanceId)
                return;

              float x  = transform.position.x;
              float y  = transform.position.y;
              float hw = aabb.width;
              float hh = aabb.height;

              if (x + hw >= left && x - hw <= right && y + hh >= bottom && y - hh <= top)
                renderable.setInstanceData(index++, {transform.getModelMatrix(), color.value});
            });
        renderable.updateInstanceDataBuffer();

        // Calculate number of sides to use for each particle
        int nInstances      = index;
        int nScaleInstances = std::min(std::max(nInstances, m_minEntities), m_maxEntities);
        int nSides          = static_cast<int>(
            std::ceil(m_logScaleFactor * logf((float)nScaleInstances / m_maxEntities)));

        const std::string &shaderName =
            nSides > 2 ? "circles" : "instanced"; // draw with points if using less than 3 sides

        shaderManager.use(shaderName);
        shaderManager.setMatrix4("projectionView", projViewMatrix);
        shaderManager.setInt("nSides", nSides);

        // Draw the renderable
        glBindVertexArray(vao);
        glDrawArraysInstanced(renderMode, 0, nVertices / vertexStride, nInstances);
      });
      shaderManager.use(0);
    }

  private:
    const static int MAX_CIRCLE_SIDES = 60;

    float m_logScaleFactor;
    int m_minEntities;
    int m_maxEntities;
  };
} // namespace RenderingBenchmark::Systems