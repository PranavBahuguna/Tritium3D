#pragma once

#include "Scenes/ParticlesBoxScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Systems/BoxContainerSystem.hpp"

#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>
#include <TritiumEngine/Utilities/Random.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Scenes;
using namespace RenderingBenchmark::Systems;
using namespace TritiumEngine::Core;
using namespace TritiumEngine::Utilities;
using namespace TritiumEngine::Rendering::TextRendering;

namespace
{
  // Simulation
  constexpr static float CONTAINER_SIZE      = 75.f;
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static glm::vec3 SHAPE_VELOCITY  = {10.f, 10.f, 0.f};
  constexpr static glm::vec3 START_POSITION  = {0.f, 0.f, 0.f};
  constexpr static float DISPLACEMENT_RADIUS = 10.f;

  // UI
  constexpr static const char *TEXT_FONT  = "Hack-Regular";
  constexpr static float TEXT_TITLE_SCALE = 0.1f;
  constexpr static float TEXT_INFO_SCALE  = 0.04f;
} // namespace

ParticlesBoxScene::ParticlesBoxScene()
    : Scene("ParticlesBox"), m_renderType(RenderType::Default), m_nParticles(1000) {}

void ParticlesBoxScene::onRegister() { setupControls(); }

void ParticlesBoxScene::onUpdate(float dt) {
  // Update UI
  static float sumDt = 0.f;
  if (sumDt < 0.2f) {
    // Update elements only every 0.2 seconds
    sumDt += dt;
    return;
  }
  sumDt = 0.f;

  setText(m_fpsText, std::format("FPS:      {:3.1f}", 1.f / dt));
  setText(m_frameTimeText, std::format("Frame:    {:3.2f}ms", dt * 1000.f));
}

void ParticlesBoxScene::init() {
  setupSystems();
  setupCamera();
  setupContainer();
  setupUI();

  switch (m_renderType) {
  case RenderType::Default:
    setText(m_titleText, "Default");
    generateParticlesDefault();
    break;
  case RenderType::Instanced:
    setText(m_titleText, "Instanced");
    generateParticlesInstanced();
    break;
  case RenderType::Geometry:
    setText(m_titleText, "Geometry Shader");
    generateParticlesGeometry();
    break;
  }
}

void ParticlesBoxScene::setupSystems() {
  addSystem<RenderSystem>();
  addSystem<TextRenderSystem>();
  addSystem<BoxContainerSystem>(CONTAINER_SIZE);
}

void ParticlesBoxScene::setupControls() {
  auto &window = m_app->window;

  // Render types
  window.addKeyCallback(Key::D, KeyState::RELEASED,
                        [this]() { setRenderType(RenderType::Default); });
  window.addKeyCallback(Key::I, KeyState::RELEASED,
                        [this]() { setRenderType(RenderType::Instanced); });
  window.addKeyCallback(Key::G, KeyState::RELEASED,
                        [this]() { setRenderType(RenderType::Geometry); });

  // Particle counts
  window.addKeyCallback(Key::NUM_1, KeyState::RELEASED, [this]() { setParticleCount(1); });
  window.addKeyCallback(Key::NUM_2, KeyState::RELEASED, [this]() { setParticleCount(10); });
  window.addKeyCallback(Key::NUM_3, KeyState::RELEASED, [this]() { setParticleCount(100); });
  window.addKeyCallback(Key::NUM_4, KeyState::RELEASED, [this]() { setParticleCount(1000); });
  window.addKeyCallback(Key::NUM_5, KeyState::RELEASED, [this]() { setParticleCount(10000); });
  window.addKeyCallback(Key::NUM_6, KeyState::RELEASED, [this]() { setParticleCount(100000); });
  window.addKeyCallback(Key::NUM_7, KeyState::RELEASED, [this]() { setParticleCount(1000000); });
}

void ParticlesBoxScene::setupCamera() {
  float screenWidth  = (float)m_app->window.getWidth();
  float screenHeight = (float)m_app->window.getHeight();
  float aspect       = screenWidth / screenHeight;

  m_app->registry.emplace<Camera>(m_app->sceneManager.getCurrentSceneEntity(),
                                  Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                                  SCREEN_UNITS, 0.1f, 100.0f);
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

void ParticlesBoxScene::setupUI() {
  // Stats
  m_titleText     = addText("", {0.f, (CONTAINER_SIZE / SCREEN_UNITS) + 0.1f}, 0.1f,
                            Text::Alignment::BOTTOM_CENTER);
  m_fpsText       = addText("FPS:  ", {-0.98f, 0.98f}, 0.04f, Text::Alignment::TOP_LEFT);
  m_frameTimeText = addText("Frame:", {-0.98f, 0.93f}, 0.04f, Text::Alignment::TOP_LEFT);
  addText(std::format("Entities: {}", m_nParticles), {-0.98f, 0.88f}, 0.04f,
          Text::Alignment::TOP_LEFT);

  // Help panel
  addText("Controls:           ", {-0.97f, 0.75f}, 0.08f, Text::Alignment::TOP_LEFT);
  addText("D: Default rendering", {-0.95f, 0.55f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("I: Instancing       ", {-0.95f, 0.45f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("G: Geometry shader  ", {-0.95f, 0.35f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("1: 1 entity         ", {-0.95f, 0.20f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("2: 10 entities      ", {-0.95f, 0.10f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("3: 100 entities     ", {-0.95f, 0.00f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("4: 1000 entities    ", {-0.95f, -0.1f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("5: 10000 entities   ", {-0.95f, -0.2f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("6: 100000 entities  ", {-0.95f, -0.3f}, 0.05f, Text::Alignment::TOP_LEFT);
  addText("7: 1000000 entities ", {-0.95f, -0.4f}, 0.05f, Text::Alignment::TOP_LEFT);
}

entt::entity ParticlesBoxScene::addText(const std::string &text, const glm::vec2 &position,
                                        float scale, Text::Alignment alignment) {
  entt::entity entity = m_app->registry.create();
  m_app->registry.emplace<Text>(entity, text, TEXT_FONT, scale, alignment);
  m_app->registry.emplace<Transform>(entity, glm::vec3{position.x, position.y, 0.f});
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("text"));
  m_app->registry.emplace<Color>(entity, COLOR_GREEN);
  return entity;
}

void ParticlesBoxScene::setText(entt::entity textEntity, const std::string &textString) {
  m_app->registry.get<Text>(textEntity).text = textString;
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
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
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
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
    m_app->registry.emplace<Transform>(
        instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
    m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
    m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
  }
}