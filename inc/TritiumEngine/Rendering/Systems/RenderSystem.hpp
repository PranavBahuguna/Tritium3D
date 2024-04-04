#pragma once

#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Rendering/BlendOptions.hpp>

#include <entt/core/type_traits.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera;

  template <uint32_t CameraTag> class RenderSystem : public System {
  public:
    RenderSystem(BlendOptions blendOptions) : System(), m_blendOptions(blendOptions) {}

    void update(float dt) override {
      m_blendOptions.apply();
      m_app->registry.view<Camera, entt::tag<CameraTag>>().each(
          [&](auto entity, Camera &camera) { draw(camera); });
    }

    void setBlendOptions(BlendOptions blendOptions) { m_blendOptions = blendOptions; }

    virtual void draw(const Camera &camera) const = 0;

  private:
    BlendOptions m_blendOptions;
  };
} // namespace TritiumEngine::Rendering