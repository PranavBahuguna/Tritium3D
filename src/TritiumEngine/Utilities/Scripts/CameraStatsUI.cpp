#pragma once

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Rendering/Components/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Components/Text.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/Scripts/CameraStatsUI.hpp>

namespace TritiumEngine::Utilities
{
  CameraStatsUI::CameraStatsUI(Application &app, const Camera &camera,
                               const CameraController &cameraController)
      : Scriptable(app), m_camera(camera), m_cameraController(cameraController) {
    initUI();
  }

  void CameraStatsUI::update(float dt) {
    // Elements are updated after a specified delay
    if (m_sumDt < 0.1f) {
      m_sumDt += dt;
      return;
    }
    m_sumDt = 0.f;

    auto &registry       = m_app->registry;
    const auto &position = m_camera.transform.position;
    float pitch          = glm::degrees(m_cameraController.getPitch());
    float yaw            = glm::degrees(m_cameraController.getYaw());
    float fov            = glm::degrees(m_camera.fov);

    registry.get<Text>(m_posXText).text  = std::format("x:     {:3.1f}", position.x);
    registry.get<Text>(m_posYText).text  = std::format("y:     {:3.1f}", position.y);
    registry.get<Text>(m_posZText).text  = std::format("z:     {:3.1f}", position.z);
    registry.get<Text>(m_pitchText).text = std::format("pitch: {:3.1f}", pitch);
    registry.get<Text>(m_yawText).text   = std::format("yaw:   {:3.1f}", yaw);
    registry.get<Text>(m_fovText).text   = std::format("fov:   {:3.1f}", fov);
  }

  void CameraStatsUI::onEnable(bool enable) {
    if (enable) {
      initUI();
    } else {
      destroyUI();
      m_sumDt = 0.f;
    }
  }

  void CameraStatsUI::initUI() {
    addText(m_posXText, "x:    ", {0.76f, 0.98f, 0.f});
    addText(m_posYText, "y:    ", {0.76f, 0.93f, 0.f});
    addText(m_posZText, "z:    ", {0.76f, 0.88f, 0.f});
    addText(m_pitchText, "pitch:", {0.76f, 0.83f, 0.f});
    addText(m_yawText, "yaw:  ", {0.76f, 0.78f, 0.f});
    addText(m_fovText, "fov:  ", {0.76f, 0.73f, 0.f});
  }

  void CameraStatsUI::destroyUI() {
    auto &registry = m_app->registry;
    registry.destroy(m_posXText);
    registry.destroy(m_posYText);
    registry.destroy(m_posZText);
    registry.destroy(m_pitchText);
    registry.destroy(m_yawText);
    registry.destroy(m_fovText);
  }

  void CameraStatsUI::addText(entt::entity &entity, const std::string &text, glm::vec3 position) {
    auto &registry      = m_app->registry;
    auto &shaderManager = m_app->shaderManager;

    entity = registry.create();
    registry.emplace<Text>(entity, text, "Hack-Regular", 0.04f, Text::Alignment::TOP_LEFT);
    registry.emplace<Transform>(entity, position);
    registry.emplace<Shader>(entity, shaderManager.get("text"));
    registry.emplace<Color>(entity, COLOR_GREEN);
  }
} // namespace TritiumEngine::Utilities