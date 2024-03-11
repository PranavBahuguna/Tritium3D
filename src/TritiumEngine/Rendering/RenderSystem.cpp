#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>

namespace TritiumEngine::Rendering
{
  RenderSystem::RenderSystem(BlendOptions blendOptions) : System(), m_blendOptions(blendOptions) {}

  void RenderSystem::update(float dt) {
    m_blendOptions.apply();
    const Camera &camera = m_app->registry.get<Camera>(m_app->sceneManager.getCurrentSceneEntity());
    draw(camera);
  }
} // namespace TritiumEngine::Rendering