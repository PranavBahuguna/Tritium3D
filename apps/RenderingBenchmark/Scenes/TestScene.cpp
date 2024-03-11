#pragma once

#include "Scenes/TestScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Rendering::TextRendering;

namespace
{
  constexpr static float SCREEN_UNITS = 100.f;
}

namespace RenderingBenchmark::Scenes
{
  TestScene::TestScene() : Scene("Test") {}

  void TestScene::init() {
    addSystem<TextRenderSystem>();

    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    m_app->registry.emplace<Camera>(m_app->sceneManager.getCurrentSceneEntity(),
                                    Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                                    SCREEN_UNITS, 0.1f, 100.0f);

    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Text>(entity, "Example Text", "Hack-Regular", 0.1f,
                                  Text::Alignment::CENTER);
    m_app->registry.emplace<Transform>(entity, glm::vec3{0.f, 0.f, 0.f});
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("text"));
    m_app->registry.emplace<Color>(entity, COLOR_GREEN);
  }

  void TestScene::dispose() {}
} // namespace RenderingBenchmark::Scenes