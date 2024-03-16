#pragma once

#include "Scenes/CubeScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Rendering::TextRendering;

namespace
{
  constexpr static float SCREEN_UNITS      = 100.f;
  constexpr static BlendOptions TEXT_BLEND = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
  const static Tag SCENE_CAMERA_TAG        = "sceneCamera";
  const static Tag UI_OVERLAY_CAMERA_TAG   = "uiCamera";
} // namespace

namespace RenderingBenchmark::Scenes
{
  CubeScene::CubeScene() : Scene("Cube"), m_callbackIds() {}

  void CubeScene::init() {
    addSystem<TextRenderSystem>(UI_OVERLAY_CAMERA_TAG, TEXT_BLEND);

    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    // Scene camera
    entt::entity sceneCamera = m_app->registry.create();
    m_app->registry.emplace<Camera>(sceneCamera, Camera::ProjectionType::PERSPECTIVE,
                                    SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f,
                                    Transform({0.f, 0.f, 10.f}));
    m_app->registry.emplace<Tag>(sceneCamera, SCENE_CAMERA_TAG);

    // UI overlay camera
    entt::entity uiCamera = m_app->registry.create();
    m_app->registry.emplace<Camera>(uiCamera, Camera::ProjectionType::ORTHOGRAPHIC,
                                    SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f);
    m_app->registry.emplace<Tag>(uiCamera, UI_OVERLAY_CAMERA_TAG);

    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Text>(entity, "Example Text", "Hack-Regular", 0.1f,
                                  Text::Alignment::CENTER);
    m_app->registry.emplace<Transform>(entity, glm::vec3{0.f, 0.f, 0.f});
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("text"));
    m_app->registry.emplace<Color>(entity, COLOR_GREEN);
  }

  void CubeScene::dispose() { m_app->window.removeCallbacks(m_callbackIds); }

  void CubeScene::setupControls() {
    // TBA
  }
} // namespace RenderingBenchmark::Scenes