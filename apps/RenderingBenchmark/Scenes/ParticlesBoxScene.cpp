#pragma once

#include "Scenes/ParticlesBoxScene.hpp"
#include "Components/Tags.hpp"
#include "Settings.hpp"
#include "Systems/BoxContainerSystem.hpp"

#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Core/Components/Rigidbody.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/Systems/InstancedRenderSystem.hpp>
#include <TritiumEngine/Rendering/Systems/StandardRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/Systems/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/CameraController.hpp>
#include <TritiumEngine/Utilities/Random.hpp>
#include <TritiumEngine/Utilities/Scripts/FpsStatsUI.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Scenes;
using namespace RenderingBenchmark::Systems;
using namespace RenderingBenchmark::Settings;
using namespace TritiumEngine::Core;
using namespace TritiumEngine::Utilities;

using Projection = Camera::Projection;

namespace
{
  constexpr static float CONTAINER_SIZE      = 750.f;
  constexpr static float SCREEN_UNITS        = 1080.f;
  constexpr static glm::vec3 SHAPE_VELOCITY  = {100.f, 100.f, 0.f};
  constexpr static glm::quat SHAPE_ROTATION  = {1.0f, 0.f, 0.f, 0.f};
  constexpr static glm::vec3 SHAPE_SCALE     = {10.f, 10.f, 1.f};
  constexpr static float DISPLACEMENT_RADIUS = 100.f;
  constexpr static BlendOptions ENTITY_BLEND = {true, GL_SRC_COLOR, GL_DST_COLOR};
  constexpr static BlendOptions TEXT_BLEND   = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
} // namespace

namespace RenderingBenchmark::Scenes
{
  ParticlesBoxScene::ParticlesBoxScene(const std::string &name, Application &app)
      : Scene(name, app), m_renderType(RenderType::Default), m_nParticles(1000),
        m_cameraController(app.inputManager), m_callbacks() {
    // Setup camera controller
    m_cameraController.mapKey(Key::LEFT, CameraAction::MOVE_LEFT);
    m_cameraController.mapKey(Key::RIGHT, CameraAction::MOVE_RIGHT);
    m_cameraController.mapKey(Key::DOWN, CameraAction::MOVE_DOWN);
    m_cameraController.mapKey(Key::UP, CameraAction::MOVE_UP);
    m_cameraController.mapKey(Key::Z, CameraAction::ZOOM_IN);
    m_cameraController.mapKey(Key::X, CameraAction::ZOOM_OUT);

    m_cameraController.moveSpeed = 40.f;
  }

  void ParticlesBoxScene::init() {
    // Setup systems
    addSystem<StandardRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<InstancedRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<TextRenderSystem<UiCameraTag::value>>(TEXT_BLEND);
    addSystem<BoxContainerSystem>(CONTAINER_SIZE);

    auto &registry = m_app.registry;
    auto &input    = m_app.inputManager;

    // Setup cameras
    float aspect      = m_app.window.getFrameAspect();
    float camWidth    = VERTICAL_SCREEN_UNITS * m_app.window.getFrameAspect();
    float camHeight   = VERTICAL_SCREEN_UNITS;
    const auto camPos = glm::vec3{0.f, 0.f, 1.f};

    auto mainCamera = registry.create();
    auto &mainCameraComponent =
        registry.emplace<Camera>(mainCamera, Projection::ORTHOGRAPHIC, camWidth, camHeight, camPos);
    registry.emplace<MainCameraTag>(mainCamera);
    m_cameraController.init(mainCameraComponent, false, false);

    auto uiCamera = registry.create();
    registry.emplace<Camera>(uiCamera, Projection::ORTHOGRAPHIC, camWidth, camHeight, camPos);
    registry.emplace<UiCameraTag>(uiCamera);

    // Setup UI
    m_titleText = addText("", {0.f, 0.82f}, 0.6f, Text::Alignment::BOTTOM_CENTER);

    // Help panel
    addText("Controls:            ", {-0.97f, 0.75f}, 0.6f, Text::Alignment::TOP_LEFT);
    addText("D: Default rendering ", {-0.95f, 0.6f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("I: Instancing        ", {-0.95f, 0.5f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("G: Geometry shader   ", {-0.95f, 0.4f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("1: 1 particle        ", {-0.95f, 0.25f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("2: 10 particles      ", {-0.95f, 0.15f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("3: 100 particles     ", {-0.95f, 0.05f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("4: 1000 particles    ", {-0.95f, -0.05f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("5: 10000 particles   ", {-0.95f, -0.15f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("6: 100000 particles  ", {-0.95f, -0.25f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("7: 1000000 particles ", {-0.95f, -0.35f}, 0.5f, Text::Alignment::TOP_LEFT);
    addText("F: Toggle FPS display", {-0.95f, -0.5f}, 0.5f, Text::Alignment::TOP_LEFT);

    // Fps stats
    auto fpsStatsUI = registry.create();
    auto &script = registry.emplace<NativeScript>(fpsStatsUI, std::make_unique<FpsStatsUI>(m_app));
    script.getInstance().setEnabled(false);

    // Controls - render types
    m_callbacks[0] = input.addKeyCallback(Key::D, KeyState::RELEASED,
                                          [this]() { setRenderType(RenderType::Default); });
    m_callbacks[1] = input.addKeyCallback(Key::I, KeyState::RELEASED,
                                          [this]() { setRenderType(RenderType::Instanced); });
    m_callbacks[2] = input.addKeyCallback(Key::G, KeyState::RELEASED,
                                          [this]() { setRenderType(RenderType::Geometry); });

    // Controls - particle counts
    m_callbacks[3] =
        input.addKeyCallback(Key::NUM_1, KeyState::RELEASED, [this]() { setParticleCount(1); });
    m_callbacks[4] =
        input.addKeyCallback(Key::NUM_2, KeyState::RELEASED, [this]() { setParticleCount(10); });
    m_callbacks[5] =
        input.addKeyCallback(Key::NUM_3, KeyState::RELEASED, [this]() { setParticleCount(100); });
    m_callbacks[6] =
        input.addKeyCallback(Key::NUM_4, KeyState::RELEASED, [this]() { setParticleCount(1000); });
    m_callbacks[7] =
        input.addKeyCallback(Key::NUM_5, KeyState::RELEASED, [this]() { setParticleCount(10000); });
    m_callbacks[8] = input.addKeyCallback(Key::NUM_6, KeyState::RELEASED,
                                          [this]() { setParticleCount(100000); });
    m_callbacks[9] = input.addKeyCallback(Key::NUM_7, KeyState::RELEASED,
                                          [this]() { setParticleCount(1000000); });

    // FPS display toggle
    m_callbacks[10] = input.addKeyCallback(Key::F, KeyState::RELEASED, [&registry, fpsStatsUI]() {
      registry.get<NativeScript>(fpsStatsUI).getInstance().toggleEnabled();
    });

    // Setup environment
    setupContainer();
    setupParticles();
  }

  void ParticlesBoxScene::dispose() {
    m_cameraController.dispose();
    m_app.inputManager.removeCallbacks(m_callbacks);
  }

  void ParticlesBoxScene::setupContainer() {
    float halfSize = (CONTAINER_SIZE + 1.f) * 0.5f;
    float right    = halfSize;
    float left     = -halfSize;
    float top      = halfSize;
    float bottom   = -halfSize;

    createWall(left, top, left, bottom);     // left
    createWall(right, top, right, bottom);   // right
    createWall(left, top, right, top);       // top
    createWall(left, bottom, right, bottom); // bottom
  }

  void ParticlesBoxScene::setupParticles() {
    auto &registry = m_app.registry;
    switch (m_renderType) {
    case RenderType::Default:
      registry.get<Text>(m_titleText).text = std::format("Default, {} particles", m_nParticles);
      generateParticlesDefault();
      break;
    case RenderType::Instanced:
      registry.get<Text>(m_titleText).text = std::format("Instanced, {} particles", m_nParticles);
      generateParticlesInstanced();
      break;
    case RenderType::Geometry:
      registry.get<Text>(m_titleText).text = std::format("Geometry, {} particles", m_nParticles);
      generateParticlesGeometry();
      break;
    }
  }

  void ParticlesBoxScene::setRenderType(RenderType renderType) {
    m_renderType = renderType;
    m_app.sceneManager.reloadCurrentScene();
  }

  void ParticlesBoxScene::setParticleCount(int nParticles) {
    m_nParticles = nParticles;
    m_app.sceneManager.reloadCurrentScene();
  }

  entt::entity ParticlesBoxScene::addText(const std::string &text, const glm::vec2 &position,
                                          float scaleFactor, Text::Alignment alignment) {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    auto entity = registry.create();
    registry.emplace<Text>(entity, text, "Hack-Regular", scaleFactor, alignment);
    registry.emplace<Transform>(entity, glm::vec3{position.x, position.y, 0.f});
    registry.emplace<Shader>(entity, shaderManager.get("text"));
    registry.emplace<Color>(entity, COLOR_GREEN);
    return entity;
  }

  void ParticlesBoxScene::createWall(float aX, float aY, float bX, float bY) {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    auto entity = registry.create();
    registry.emplace<Transform>(entity);
    registry.emplace<Renderable>(entity, GL_LINES, Primitives::createLine(aX, aY, bX, bY));
    registry.emplace<Shader>(entity, shaderManager.get("default"));
    registry.emplace<Color>(entity, COLOR_WHITE);
  }

  void ParticlesBoxScene::generateParticlesDefault() {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    for (int i = 0; i < m_nParticles; ++i) {
      auto entity = registry.create();
      registry.emplace<Transform>(entity, RandomUtils::RadialPosition(DISPLACEMENT_RADIUS, true),
                                  SHAPE_ROTATION, SHAPE_SCALE);
      registry.emplace<Renderable>(entity, GL_TRIANGLES, Primitives::createQuad());
      registry.emplace<Shader>(entity, shaderManager.get("default"));
      registry.emplace<Color>(entity, COLOR_RED);
      registry.emplace<Rigidbody>(entity, SHAPE_VELOCITY);
    }
  }

  void ParticlesBoxScene::generateParticlesInstanced() {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    // Create instanced renderable template
    auto entity      = registry.create();
    auto &renderable = registry.emplace<InstancedRenderable>(
        entity, GL_TRIANGLES, Primitives::createQuad(), m_nParticles);
    registry.emplace<Shader>(entity, shaderManager.get("instanced"));

    // Add instances
    for (int i = 0; i < m_nParticles; ++i) {
      auto instancedEntity = registry.create();
      registry.emplace<InstanceTag>(instancedEntity, renderable.getInstanceId());
      registry.emplace<Transform>(instancedEntity,
                                  RandomUtils::RadialPosition(DISPLACEMENT_RADIUS, true),
                                  SHAPE_ROTATION, SHAPE_SCALE);
      registry.emplace<Color>(instancedEntity, COLOR_RED);
      registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
    }
  }

  void ParticlesBoxScene::generateParticlesGeometry() {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    // Create instanced renderable template
    auto entity      = registry.create();
    auto &renderable = registry.emplace<InstancedRenderable>(
        entity, GL_POINTS, Primitives::createPoint2d(), m_nParticles);
    registry.emplace<Shader>(entity, shaderManager.get("geometry"));

    // Add instances
    for (int i = 0; i < m_nParticles; ++i) {
      auto instancedEntity = registry.create();
      registry.emplace<InstanceTag>(instancedEntity, renderable.getInstanceId());
      registry.emplace<Transform>(instancedEntity,
                                  RandomUtils::RadialPosition(DISPLACEMENT_RADIUS, true),
                                  SHAPE_ROTATION, SHAPE_SCALE);
      registry.emplace<Color>(instancedEntity, COLOR_RED);
      registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
    }
  }
} // namespace RenderingBenchmark::Scenes