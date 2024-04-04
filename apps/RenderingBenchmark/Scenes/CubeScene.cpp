#pragma once

#include "Scenes/CubeScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Scripts/CameraStatsUI.hpp"
#include "Components/Scripts/FpsStatsUI.hpp"
#include "Components/Tags.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/NativeScript.hpp>
#include <TritiumEngine/Rendering/InstancedRenderSystem.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/StandardRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/Random.hpp>

#include <glm/gtc/constants.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Scripts;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Rendering::TextRendering;
using namespace TritiumEngine::Utilities;

namespace
{
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static BlendOptions TEXT_BLEND   = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
  constexpr static BlendOptions ENTITY_BLEND = {true, GL_SRC_COLOR, GL_DST_COLOR};
  constexpr static glm::vec3 CAMERA_POSITION = {0.f, 0.f, 200.f};
} // namespace

namespace RenderingBenchmark::Scenes
{
  CubeScene::CubeScene() : Scene("Cube"), m_callbacks() {}

  void CubeScene::init() {
    setupSystems();
    setupUI();
    setupCameras();
    setupControls();
    generateCubes(1000);
  }

  void CubeScene::onRegister() { setupCameraController(); }

  void CubeScene::dispose() {
    m_cameraController.dispose();
    m_app->window.removeCallbacks(m_callbacks);
  }

  void CubeScene::setupSystems() {
    addSystem<StandardRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<InstancedRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<TextRenderSystem<UiCameraTag::value>>(TEXT_BLEND);
  }

  void CubeScene::setupUI() {
    auto &registry = m_app->registry;

    m_fpsStatsUI = registry.create();
    auto &fpsScript =
        registry.emplace<NativeScript>(m_fpsStatsUI, std::make_unique<FpsStatsUI>(*m_app));
    fpsScript.getInstance().setEnabled(false);

    m_cameraStatsUI = registry.create();
    auto &cameraScript =
        registry.emplace<NativeScript>(m_cameraStatsUI, std::make_unique<CameraStatsUI>(*m_app));
    cameraScript.getInstance().setEnabled(false);
  }

  void CubeScene::setupCameras() {
    auto &registry     = m_app->registry;
    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    // Scene camera
    entt::entity sceneCamera   = registry.create();
    auto &sceneCameraComponent = registry.emplace<Camera>(
        sceneCamera, Camera::ProjectionType::PERSPECTIVE, SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f,
        1000.0f, Transform(CAMERA_POSITION));
    registry.emplace<MainCameraTag>(sceneCamera);
    m_cameraController.init(sceneCameraComponent);

    // UI overlay camera
    entt::entity uiCamera = registry.create();
    registry.emplace<Camera>(uiCamera, Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                             SCREEN_UNITS, 0.1f, 100.0f);
    registry.emplace<UiCameraTag>(uiCamera);
  }

  void CubeScene::setupCameraController() {
    m_cameraController.registerWindow(m_app->window);
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

  void CubeScene::setupControls() {
    // FPS/camera stats display toggles
    auto &window   = m_app->window;
    m_callbacks[0] = window.addKeyCallback(Key::F, KeyState::RELEASED, [this]() {
      m_app->registry.get<NativeScript>(m_fpsStatsUI).getInstance().toggleEnabled();
    });
    m_callbacks[1] = window.addKeyCallback(Key::M, KeyState::RELEASED, [this]() {
      m_app->registry.get<NativeScript>(m_cameraStatsUI).getInstance().toggleEnabled();
    });
  }

  void CubeScene::generateCubes(int nCubes) {
    entt::entity entity = m_app->registry.create();
    auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
        entity, GL_TRIANGLES, Primitives::createCube(), nCubes);
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("instanced"));

    // Add instances
    for (int i = 0; i < nCubes; ++i) {
      entt::entity instancedEntity = m_app->registry.create();
      m_app->registry.emplace<InstanceTag>(instancedEntity, renderable.getInstanceId());
      m_app->registry.emplace<Transform>(instancedEntity, randCubePosition(100.f));
      m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
      m_app->registry.emplace<Rigidbody>(instancedEntity);
    }
  }
} // namespace RenderingBenchmark::Scenes