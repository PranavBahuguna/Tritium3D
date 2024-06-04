#pragma once

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Components/Transform.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Components/Text.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/Scripts/FpsStatsUI.hpp>

namespace TritiumEngine::Utilities
{
  FpsStatsUI::FpsStatsUI(Application &app) : Scriptable(app) { initUI(); }

  void FpsStatsUI::update(float dt) {
    if (m_sumDt < 0.2f) {
      // Elements are updated after a specified delay
      m_sumDt += dt;
      ++m_nFrames;
      return;
    }
    float avgDt = m_sumDt / m_nFrames;
    m_sumDt     = 0.f;
    m_nFrames   = 0;

    m_app->registry.get<Text>(m_fpsText).text = std::format("FPS:   {:3.1f}", 1.f / avgDt);
    m_app->registry.get<Text>(m_frameTimeText).text =
        std::format("Frame: {:3.2f}ms", avgDt * 1000.f);
  }

  void FpsStatsUI::onEnable(bool enable) {
    if (enable)
      initUI();
    else {
      destroyUI();
      m_sumDt   = 0.f;
      m_nFrames = 0;
    }
  }

  void FpsStatsUI::initUI() {
    addText(m_fpsText, "FPS:", {-0.98f, 0.98f, 0.f});
    addText(m_frameTimeText, "Frame:", {-0.98f, 0.93f, 0.f});
  }

  void FpsStatsUI::destroyUI() {
    auto &registry = m_app->registry;
    registry.destroy(m_fpsText);
    registry.destroy(m_frameTimeText);
  }

  void FpsStatsUI::addText(entt::entity &entity, const std::string &text, glm::vec3 position) {
    auto &registry      = m_app->registry;
    auto &shaderManager = m_app->shaderManager;

    entity = registry.create();
    registry.emplace<Text>(entity, text, "Hack-Regular", 0.43f, Text::Alignment::TOP_LEFT);
    registry.emplace<Transform>(entity, position);
    registry.emplace<Shader>(entity, shaderManager.get("text"));
    registry.emplace<Color>(entity, COLOR_GREEN);
  }
} // namespace TritiumEngine::Utilities