#pragma once

#include "DefaultScene.hpp"
#include "BoxContainerSystem.hpp"
#include "Rigidbody.hpp"

#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Color.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>

#include <cmath>
#include <numbers>

#define SCREEN_UNITS        100.f
#define USE_INSTANCED       true
#define CONTAINER_SIZE      80.f
#define SHAPE_VELOCITY      glm::vec3(10.f, 10.f, 0.f)
#define START_POSITION      glm::vec3(-20.f, 0.f, 0.f)
#define DISPLACEMENT_RADIUS 10.f
#define NUM_SQUARES         10000

using namespace TritiumEngine::Core;

DefaultScene::DefaultScene() : Scene("DefaultScene"), m_mt(m_rd()), m_rand(0.f, 1.f) {}

void DefaultScene::init() {
  addSystem<RenderSystem>();
  addSystem<BoxContainerSystem>(CONTAINER_SIZE);

  setupCamera();
  setupContainer();
  if (!USE_INSTANCED)
    generateSquares(NUM_SQUARES);
  else
    generateSquaresInstanced(NUM_SQUARES);
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
    m_app->registry.emplace<Rigidbody>(instancedEntity, SQUARE_VELOCITY);
  }
}

glm::vec3 DefaultScene::randRadialPosition(float radius, bool uniform) {
  float a = m_rand(m_mt);
  float b = m_rand(m_mt);

  if (uniform && a > b)
    std::swap(a, b);

  float randX = b * radius * std::cos(2 * std::numbers::pi * a / b);
  float randY = b * radius * std::sin(2 * std::numbers::pi * a / b);

  return {randX, randY, 0.f};
}