#pragma once

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Rendering/RenderSettings.hpp>

#include <entt/core/type_traits.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera;

  template <uint32_t CameraTag> class RenderSystem : public System {
  public:
    RenderSystem(RenderSettings renderSettings) : System(), m_renderSettings(renderSettings) {}

    void update(float dt) override {
      m_renderSettings.apply();
      m_app->registry.view<Camera, entt::tag<CameraTag>>().each(
          [&](auto entity, Camera &camera) { draw(camera); });
    }

    void setBlendOptions(RenderSettings renderSettings) { m_renderSettings = renderSettings; }

    virtual void draw(const Camera &camera) const = 0;

  private:
    RenderSettings m_renderSettings;
  };
} // namespace TritiumEngine::Rendering