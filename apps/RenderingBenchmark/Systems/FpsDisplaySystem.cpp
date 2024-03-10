#include "Systems/FpsDisplaySystem.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering::TextRendering;
using namespace TritiumEngine::Utilities;

namespace
{
  const static char *TEXT_FONT                    = "Hack-Regular";
  const static char *TEXT_SHADER                  = "text";
  constexpr static float TEXT_SCALE               = 0.04f;
  constexpr static Text::Alignment TEXT_ALIGNMENT = Text::Alignment::TOP_LEFT;
  constexpr static uint32_t TEXT_COLOR            = COLOR_GREEN;
  constexpr static float UPDATE_DELAY             = 0.2f;
} // namespace

namespace RenderingBenchmark::Systems
{
  FpsDisplaySystem::FpsDisplaySystem() : System() {}

  void FpsDisplaySystem::init() {
    m_fpsText = m_app->registry.create();
    m_app->registry.emplace<Text>(m_fpsText, "FPS:", TEXT_FONT, TEXT_SCALE, TEXT_ALIGNMENT);
    m_app->registry.emplace<Transform>(m_fpsText, glm::vec3{-0.98f, 0.98f, 0.f});
    m_app->registry.emplace<Shader>(m_fpsText, m_app->shaderManager.get(TEXT_SHADER));
    m_app->registry.emplace<Color>(m_fpsText, TEXT_COLOR);

    m_frameTimeText = m_app->registry.create();
    m_app->registry.emplace<Text>(m_frameTimeText, "Frame:", TEXT_FONT, TEXT_SCALE, TEXT_ALIGNMENT);
    m_app->registry.emplace<Transform>(m_frameTimeText, glm::vec3{-0.98f, 0.93f, 0.f});
    m_app->registry.emplace<Shader>(m_frameTimeText, m_app->shaderManager.get(TEXT_SHADER));
    m_app->registry.emplace<Color>(m_frameTimeText, TEXT_COLOR);
  }

  void FpsDisplaySystem::dispose() {
    m_app->registry.destroy(m_fpsText);
    m_app->registry.destroy(m_frameTimeText);
  }

  void FpsDisplaySystem::update(float dt) {
    static float sumDt = 0.f;
    static int nFrames = 1;
    if (sumDt < UPDATE_DELAY) {
      // Elements are updated after a specified delay
      sumDt += dt;
      ++nFrames;
      return;
    }
    float avgDt = sumDt / nFrames;
    sumDt       = 0.f;
    nFrames     = 1;

    m_app->registry.get<Text>(m_fpsText).text = std::format("FPS:   {:3.1f}", 1.f / avgDt);
    m_app->registry.get<Text>(m_frameTimeText).text =
        std::format("Frame: {:3.2f}ms", avgDt * 1000.f);
  }
} // namespace RenderingBenchmark::Systems