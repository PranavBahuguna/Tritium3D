#pragma once

#include "Scenes/ParticlesBoxScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Systems/BoxContainerSystem.hpp"

#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Color.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Utilities/Random.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Scenes;
using namespace RenderingBenchmark::Systems;
using namespace TritiumEngine::Core;
using namespace TritiumEngine::Utilities;

namespace
{
  constexpr static float CONTAINER_SIZE      = 80.f;
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static glm::vec3 SHAPE_VELOCITY  = {10.f, 10.f, 0.f};
  constexpr static glm::vec3 START_POSITION  = {0.f, 0.f, 0.f};
  constexpr static float DISPLACEMENT_RADIUS = 10.f;
} // namespace

ParticlesBoxScene::ParticlesBoxScene()
    : Scene("ParticlesBox"), m_renderType(RenderType::Default), m_nParticles(10000) {}

void ParticlesBoxScene::onRegister() { setupControls(); }

void ParticlesBoxScene::init() {
  setupSystems();
  setupCamera();
  setupContainer();

  switch (m_renderType) {
  case RenderType::Default:
    generateParticlesDefault();
    break;
  case RenderType::Instanced:
    generateParticlesInstanced();
    break;
  case RenderType::Geometry:
    generateParticlesGeometry();
    break;
  }
}

void ParticlesBoxScene::setupSystems() {
  addSystem<RenderSystem>();
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
  float halfSize = (CONTAINER_SIZE + 1.f) / 2.f;
  float right    = halfSize;
  float left     = -halfSize;
  float top      = halfSize;
  float bottom   = -halfSize;

  createWall(left, top, left, bottom);     // left
  createWall(right, top, right, bottom);   // right
  createWall(left, top, right, top);       // top
  createWall(left, bottom, right, bottom); // bottom
}

void ParticlesBoxScene::setRenderType(RenderType renderType) {
  m_renderType = renderType;
  m_app->sceneManager.reloadCurrentScene();
}

void ParticlesBoxScene::setParticleCount(size_t nParticles) {
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
  for (size_t i = 0; i < m_nParticles; ++i) {
    glm::vec3 displacement = randRadialPosition(DISPLACEMENT_RADIUS, true);

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
      entity, GL_TRIANGLES, Primitives::createSquare(), static_cast<GLsizei>(m_nParticles));
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("instanced"));

  // Add instances
  for (size_t i = 0; i < m_nParticles; ++i) {
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
      entity, GL_POINTS, Primitives::createPoint(0.f, 0.f), static_cast<GLsizei>(m_nParticles));
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("geometry"));

  // Add instances
  for (size_t i = 0; i < m_nParticles; ++i) {
    entt::entity instancedEntity = m_app->registry.create();
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
    m_app->registry.emplace<Transform>(
        instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
    m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
    m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
  }
}