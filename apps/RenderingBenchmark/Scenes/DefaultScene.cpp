#pragma once

#include "Scenes/DefaultScene.hpp"
#include "Components/Rigidbody.hpp"
#include "Config.hpp"
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

DefaultScene::DefaultScene() : Scene("DefaultScene") {}

void DefaultScene::init() {
  addSystem<RenderSystem>();
  addSystem<BoxContainerSystem>(CONTAINER_SIZE);

  setupCamera();
  setupContainer();

  switch (RENDERING_TYPE) {
  case RenderingType::Default:
    generateSquares(NUM_SHAPES);
    break;
  case RenderingType::Instanced:
    generateSquaresInstanced(NUM_SHAPES);
    break;
  case RenderingType::Geometry:
    generateSquaresGeometry(NUM_SHAPES);
    break;
  }
}

void DefaultScene::setupCamera() {
  float screenWidth  = (float)m_app->window.getWidth();
  float screenHeight = (float)m_app->window.getHeight();
  float aspect       = screenWidth / screenHeight;

  m_app->registry.emplace<Camera>(m_app->sceneManager.getCurrentSceneEntity(),
                                  Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                                  SCREEN_UNITS, 0.1f, 100.0f);
}

void DefaultScene::setupContainer() {
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

void DefaultScene::createWall(float aX, float aY, float bX, float bY) {
  entt::entity entity = m_app->registry.create();
  m_app->registry.emplace<Transform>(entity);
  m_app->registry.emplace<Renderable>(entity, GL_LINES, Primitives::createLine(aX, aY, bX, bY));
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("default"));
  m_app->registry.emplace<Color>(entity, 0xFFFFFFFF);
}

void DefaultScene::generateSquares(size_t n) {
  for (size_t i = 0; i < n; ++i) {
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

void DefaultScene::generateSquaresInstanced(size_t n) {
  // Create instanced renderable template
  entt::entity entity = m_app->registry.create();
  auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
      entity, GL_TRIANGLES, Primitives::createSquare(), static_cast<GLsizei>(n));
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("instanced"));

  // Add instances
  for (size_t i = 0; i < n; ++i) {
    entt::entity instancedEntity = m_app->registry.create();
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
    m_app->registry.emplace<Transform>(
        instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
    m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
    m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
  }
}

void DefaultScene::generateSquaresGeometry(size_t n) {
  // Create instanced renderable template
  entt::entity entity = m_app->registry.create();
  auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
      entity, GL_POINTS, Primitives::createPoint(0.f, 0.f), static_cast<GLsizei>(n));
  m_app->registry.emplace<Shader>(entity, m_app->shaderManager.get("geometry"));

  // Add instances
  for (size_t i = 0; i < n; ++i) {
    entt::entity instancedEntity = m_app->registry.create();
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
    m_app->registry.emplace<Transform>(
        instancedEntity, START_POSITION + randRadialPosition(DISPLACEMENT_RADIUS, true));
    m_app->registry.emplace<Color>(instancedEntity, 0xFF0000FF);
    m_app->registry.emplace<Rigidbody>(instancedEntity, SHAPE_VELOCITY);
  }
}