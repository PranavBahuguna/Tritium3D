#pragma once

#include "FallingSquaresScene.hpp"
#include "Gravity.hpp"
#include "Rigidbody.hpp"

#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Color.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>

#define SCREEN_UNITS     100.0f
#define USE_INSTANCED    true
#define GRAVITY_STRENGTH 0.08f
#define NUM_SQUARES      10000

using namespace TritiumEngine::Core;

FallingSquaresScene::FallingSquaresScene()
    : Scene("FallingSquares"), m_mt(m_rd()), m_rand(0.f, 1.f), m_rand256(0, 255) {}

void FallingSquaresScene::init() {
  addSystem<RenderSystem>();
  addSystem<Gravity>(GRAVITY_STRENGTH);

  setupCamera();
  if (!USE_INSTANCED)
    generateSquares(NUM_SQUARES);
  else
    generateSquaresInstanced(NUM_SQUARES);
}

void FallingSquaresScene::setupCamera() {
  float screenWidth  = (float)m_app->window.getWidth();
  float screenHeight = (float)m_app->window.getHeight();
  float aspect       = screenWidth / screenHeight;

  m_app->registry.emplace<Camera>(m_app->sceneManager.getCurrentSceneEntity(),
                                  Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                                  SCREEN_UNITS, 0.1f, 100.0f);
}

void FallingSquaresScene::generateSquares(size_t n) {
  ShaderId shaderId = m_app->shaderManager.get("default");

  for (size_t i = 0; i < n; ++i) {
    glm::vec3 displacement{(m_rand(m_mt) - 0.5f) * 100.f, (m_rand(m_mt) - 0.5f) * 100.f, 0.f};
    Color color{0};
    color.value |= m_rand256(m_mt);       // R
    color.value |= m_rand256(m_mt) << 8;  // G
    color.value |= m_rand256(m_mt) << 16; // B
    color.value |= 255 << 24;             // A

    entt::entity entity = m_app->registry.create();
    m_app->registry.emplace<Transform>(entity, m_startPos + displacement);
    m_app->registry.emplace<Renderable>(entity, GL_TRIANGLES, m_squareRenderData);
    m_app->registry.emplace<Shader>(entity, shaderId);
    m_app->registry.emplace<Color>(entity, color);
    m_app->registry.emplace<Rigidbody>(entity, glm::vec3());
  }
}

void FallingSquaresScene::generateSquaresInstanced(size_t n) {
  // Setup square properties
  ShaderId shaderId = m_app->shaderManager.get("instanced");

  // Create instanced renderable template
  entt::entity entity = m_app->registry.create();
  auto &renderable    = m_app->registry.emplace<InstancedRenderable>(
      entity, GL_TRIANGLES, m_squareRenderData, static_cast<GLsizei>(n));
  m_app->registry.emplace<Shader>(entity, shaderId);

  // Add instances
  for (size_t i = 0; i < n; ++i) {
    glm::vec3 displacement{(m_rand(m_mt) - 0.5f) * 100.f, (m_rand(m_mt) - 0.5f) * 100.f, 0.f};
    Color color{0};
    color.value |= m_rand256(m_mt);       // R
    color.value |= m_rand256(m_mt) << 8;  // G
    color.value |= m_rand256(m_mt) << 16; // B
    color.value |= 255 << 24;             // A

    entt::entity instancedEntity = m_app->registry.create();
    m_app->registry.emplace<InstancedRenderableTag>(instancedEntity, renderable.getInstanceId());
    m_app->registry.emplace<Transform>(instancedEntity, m_startPos + displacement);
    m_app->registry.emplace<Rigidbody>(instancedEntity, glm::vec3());
    m_app->registry.emplace<Color>(instancedEntity, color);
  }
}