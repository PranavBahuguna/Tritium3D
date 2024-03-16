#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>

namespace TritiumEngine::Rendering
{
  RenderSystem::RenderSystem(Tag cameraTag, BlendOptions blendOptions)
      : System(), m_cameraTag(cameraTag), m_blendOptions(blendOptions) {}

  void RenderSystem::update(float dt) {
    m_blendOptions.apply();
    m_app->registry.view<Camera, Tag>().each([&](auto entity, Camera &camera, Tag &tag) {
      if (tag == m_cameraTag)
        draw(camera);
    });
  }
} // namespace TritiumEngine::Rendering