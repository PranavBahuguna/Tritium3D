#pragma once

#include "Scenes/ParticlesBoxScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Scripts/FpsStatsUI.hpp"
#include "Components/Tags.hpp"
#include "Systems/BoxContainerSystem.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/NativeScript.hpp>
#include <TritiumEngine/Rendering/InstancedRenderSystem.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/StandardRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/Random.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Scenes;
using namespace RenderingBenchmark::Systems;
using namespace RenderingBenchmark::Scripts;
using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering::TextRendering;

namespace
{
  constexpr static float CONTAINER_SIZE      = 75.f;
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static glm::vec3 SHAPE_VELOCITY  = {10.f, 10.f, 0.f};
  constexpr static glm::vec3 START_POSITION  = {0.f, 0.f, 0.f};
  constexpr static float DISPLACEMENT_RADIUS = 10.f;
  constexpr static BlendOptions ENTITY_BLEND = {true, GL_SRC_COLOR, GL_DST_COLOR};
  constexpr static BlendOptions TEXT_BLEND   = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
} // namespace

namespace RenderingBenchmark::Scenes
{
  ParticlesBoxScene::ParticlesBoxScene()
      : Scene("ParticlesBox"), m_renderType(RenderType::Default), m_nParticles(1000),
        m_callbacks() {}

  void ParticlesBoxScene::init() {
    setupSystems();
    initUI();
    setupControls();
    setupCamera();
    setupContainer();
    setupParticles();
  }

  void ParticlesBoxScene::dispose() { m_app->window.removeCallbacks(m_callbacks); }

  void ParticlesBoxScene::setupSystems() {
    addSystem<StandardRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<InstancedRenderSystem<MainCameraTag::value>>(ENTITY_BLEND);
    addSystem<TextRenderSystem<MainCameraTag::value>>(TEXT_BLEND);
    addSystem<BoxContainerSystem>(CONTAINER_SIZE);
  }

  void ParticlesBoxScene::setupControls() {
    auto &window = m_app->window;

    // Render types
    m_callbacks[0] = window.addKeyCallback(Key::D, KeyState::RELEASED,
                                           [this]() { setRenderType(RenderType::Default); });
    m_callbacks[1] = window.addKeyCallback(Key::I, KeyState::RELEASED,
                                           [this]() { setRenderType(RenderType::Instanced); });
    m_callbacks[2] = window.addKeyCallback(Key::G, KeyState::RELEASED,
                                           [this]() { setRenderType(RenderType::Geometry); });

    // Particle counts
    m_callbacks[3] =
        window.addKeyCallback(Key::NUM_1, KeyState::RELEASED, [this]() { setParticleCount(1); });
    m_callbacks[4] =
        window.addKeyCallback(Key::NUM_2, KeyState::RELEASED, [this]() { setParticleCount(10); });
    m_callbacks[5] =
        window.addKeyCallback(Key::NUM_3, KeyState::RELEASED, [this]() { setParticleCount(100); });
    m_callbacks[6] =
        window.addKeyCallback(Key::NUM_4, KeyState::RELEASED, [this]() { setParticleCount(1000); });
    m_callbacks[7] = window.addKeyCallback(Key::NUM_5, KeyState::RELEASED,
                                           [this]() { setParticleCount(10000); });
    m_callbacks[8] = window.addKeyCallback(Key::NUM_6, KeyState::RELEASED,
                                           [this]() { setParticleCount(100000); });
    m_callbacks[9] = window.addKeyCallback(Key::NUM_7, KeyState::RELEASED,
                                           [this]() { setParticleCount(1000000); });

    // FPS display toggle
    m_callbacks[10] = window.addKeyCallback(Key::F, KeyState::RELEASED, [this]() {
      m_app->registry.get<NativeScript>(m_fpsStatsUI).getInstance().toggleEnabled();
    });
  }

  void ParticlesBoxScene::setupCamera() {
    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Camera>(entity, Camera::ProjectionType::ORTHOGRAPHIC,
                                    SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f);
    m_app->registry.emplace<MainCameraTag>(entity);
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

  void ParticlesBoxScene::initUI() {
    m_titleText = addText("", {0.f, 0.82f}, 0.055f, Text::Alignment::BOTTOM_CENTER);

    // Help panel
    addText("Controls:            ", {-0.97f, 0.75f}, 0.055f, Text::Alignment::TOP_LEFT);
    addText("D: Default rendering ", {-0.95f, 0.6f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("I: Instancing        ", {-0.95f, 0.5f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("G: Geometry shader   ", {-0.95f, 0.4f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("1: 1 particle        ", {-0.95f, 0.25f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("2: 10 particles      ", {-0.95f, 0.15f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("3: 100 particles     ", {-0.95f, 0.05f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("4: 1000 particles    ", {-0.95f, -0.05f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("5: 10000 particles   ", {-0.95f, -0.15f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("6: 100000 particles  ", {-0.95f, -0.25f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("7: 1000000 particles ", {-0.95f, -0.35f}, 0.045f, Text::Alignment::TOP_LEFT);
    addText("F: Toggle FPS display", {-0.95f, -0.5f}, 0.045f, Text::Alignment::TOP_LEFT);

    auto &registry = m_app->registry;
    m_fpsStatsUI   = registry.create();
    auto &script =
        registry.emplace<NativeScript>(m_fpsStatsUI, std::make_unique<FpsStatsUI>(*m_app));
    // script.getInstance().setEnabled(false);
  }

  void ParticlesBoxScene::setupParticles() {
    switch (m_renderType) {
    case RenderType::Default:
      m_app->registry.get<Text>(m_titleText).text =
          std::format("Default, {} particles", m_nParticles);
      generateParticlesDefault();
      break;
    case RenderType::Instanced:
      m_app->registry.get<Text>(m_titleText).text =
          std::format("Instanced, {} particles", m_nParticles);
      generateParticlesInstanced();
      break;
    case RenderType::Geometry:
      m_app->registry.get<Text>(m_titleText).text =
          std::format("Geometry Shader, {} particles", m_nParticles);
      generateParticlesGeometry();
      break;
    }
  }

  entt::entity ParticlesBoxScene::addText(const std::string &text, const glm::vec2 &position,
                                          float scale, Text::Alignment alignment) {
    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Text>(entity, text, "Hack-Regular", scale, alignment);
    m_app->registry.emplace<Transform>(entity, glm::vec3{position.x, position.y, 0.f});
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("text"));
    m_app->registry.emplace<Color>(entity, COLOR_GREEN);
    return entity;
  }

  void ParticlesBoxScene::setRenderType(RenderType renderType) {
    m_renderType = renderType;
    m_app->sceneManager.reloadCurrentScene();
  }

  void ParticlesBoxScene::setParticleCount(int nParticles) {
    m_nParticles = nParticles;
    m_app->sceneManager.reloadCurrentScene();
  }

  void ParticlesBoxScene::createWall(float aX, float aY, float bX, float bY) {
    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Transform>(entity);
    m_app->registry.emplace<Renderable>(entity, GL_LINES, Primitives::createLine(aX, aY, bX, bY));
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("default"));
    m_app->registry.emplace<Color>(entity, 0xFFFFFFFF);
  }

  void ParticlesBoxScene::generateParticlesDefault() {
    for (int i = 0; i < m_nParticles; ++i) {
      entt::entity entity = m_app->registry.create();
      m_app->registry.emplace<Transform>(entity, START_POSITION +
                                                     randRadialPosition(DISPLACEMENT_RADIUS, true));
      m_app->registry.emplace<Renderable>(entity, GL_TRIANGLES, Primitives::createSquare());
      m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("default"));
      m_app->registry.emplace<Color>(entity, 0xFF0000FF);
      m_app->registry.emplace<Rigidbody>(entity, SHAPE_VELOCITY);
    }
  }

  void ParticlesBoxScene::generateParticlesInstanced() {
    // Create instanced renderable template
    entt::entity entity = m_app->registry.create();
    auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
        entity, GL_TRIANGLES, Primitives::createSquare(), m_nParticles);
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("instanced"));

    // Add instances
    for (int i = 0; i < m_nParticles; ++i) {
      entt::entity instancedEntity = m_app->registry.create();
      m_app->registry.emplace<InstanceTag>(instancedEntity, renderable.getInstanceId());
      m_app->registry.emplace<Transform>(
          instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
      m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
      m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
    }
  }

  void ParticlesBoxScene::generateParticlesGeometry() {
    // Create instanced renderable template
    entt::entity entity = m_app->registry.create();
    auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
        entity, GL_POINTS, Primitives::createPoint(0.f, 0.f), m_nParticles);
    m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("geometry"));

    // Add instances
    for (int i = 0; i < m_nParticles; ++i) {
      entt::entity instancedEntity = m_app->registry.create();
      m_app->registry.emplace<InstanceTag>(instancedEntity, renderable.getInstanceId());
      m_app->registry.emplace<Transform>(
          instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
      m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
      m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
    }
  }
} // namespace RenderingBenchmark::Scenes