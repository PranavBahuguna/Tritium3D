#pragma once

#include "Scenes/CubeScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Systems/FpsDisplaySystem.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Tag.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/StandardRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/Random.hpp>

#include <glm/gtc/constants.hpp>

using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Rendering::TextRendering;
using namespace TritiumEngine::Utilities;
using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Systems;

namespace
{
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static BlendOptions TEXT_BLEND   = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
  constexpr static BlendOptions ENTITY_BLEND = {true, GL_SRC_COLOR, GL_DST_COLOR};
  const static std::string MAIN_CAMERA_TAG   = "mainCamera";
  const static std::string UI_CAMERA_TAG     = "uiCamera";
} // namespace

namespace RenderingBenchmark::Scenes
{
  CubeScene::CubeScene() : Scene("Cube") {}

  void CubeScene::init() {
    setupSystems();
    setupCameras();
    m_cameraController.init(m_app->registry.get<Camera>(m_sceneCamera));

    int nParticles      = 1000;
    entt::entity entity = m_app->registry.create();
    auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
        entity, GL_TRIANGLES, Primitives::createSquare(), nParticles);
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("instanced"));

    // Add instances
    for (int i = 0; i < nParticles; ++i) {
      entt::entity instancedEntity = m_app->registry.create();
      m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
      m_app->registry.emplace<Transform>(instancedEntity, randRadialPosition(20.f, true));
      m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
      m_app->registry.emplace<Rigidbody>(instancedEntity);
    }
  }

  void CubeScene::onRegister() {
    m_cameraController.registerWindow(m_app->window);
    setupControls();
  }

  void CubeScene::dispose() { m_cameraController.dispose(); }

  void CubeScene::setupSystems() {
    addSystem<StandardRenderSystem>(MAIN_CAMERA_TAG, ENTITY_BLEND);
    addSystem<TextRenderSystem>(UI_CAMERA_TAG, TEXT_BLEND);
    addSystem<FpsDisplaySystem>();
  }

  void CubeScene::setupCameras() {
    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    // Scene camera
    m_sceneCamera = m_app->registry.create();
    m_app->registry.emplace<Camera>(m_sceneCamera, Camera::ProjectionType::PERSPECTIVE,
                                    SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f,
                                    Transform({0.f, 0.f, 10.f}));
    m_app->registry.emplace<Tag>(m_sceneCamera, MAIN_CAMERA_TAG);

    // UI overlay camera
    entt::entity uiCamera = m_app->registry.create();
    m_app->registry.emplace<Camera>(uiCamera, Camera::ProjectionType::ORTHOGRAPHIC,
                                    SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f);
    m_app->registry.emplace<Tag>(uiCamera, UI_CAMERA_TAG);
  }

  void CubeScene::setupControls() {
    m_cameraController.mapKey(Key::W, CameraAction::MOVE_FORWARD);
    m_cameraController.mapKey(Key::A, CameraAction::MOVE_LEFT);
    m_cameraController.mapKey(Key::S, CameraAction::MOVE_BACKWARD);
    m_cameraController.mapKey(Key::D, CameraAction::MOVE_RIGHT);
    m_cameraController.mapKey(Key::Q, CameraAction::MOVE_UP);
    m_cameraController.mapKey(Key::E, CameraAction::MOVE_DOWN);
    m_cameraController.mapKey(Key::LEFT, CameraAction::TURN_LEFT);
    m_cameraController.mapKey(Key::RIGHT, CameraAction::TURN_RIGHT);
    m_cameraController.mapKey(Key::UP, CameraAction::TURN_UP);
    m_cameraController.mapKey(Key::DOWN, CameraAction::TURN_DOWN);
    m_cameraController.mapKey(Key::Z, CameraAction::ZOOM_IN);
    m_cameraController.mapKey(Key::X, CameraAction::ZOOM_OUT);
    m_cameraController.mapKey(Key::C, CameraAction::CENTER);
  }
} // namespace RenderingBenchmark::Scenes