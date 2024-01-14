#pragma once

#include "Gravity.hpp"

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <random>

#define SCREEN_UNITS 100.0f

using namespace TritiumEngine::Core;

class FallingSquaresScene : public Scene {
public:
  FallingSquaresScene() : Scene("FallingSquares") {}

protected:
  void init() override {
    setupCamera();
    generateSquares(10);
  }

private:
  void setupCamera() {
    float screenWidth  = (float)m_app->window.getWidth();
    float screenHeight = (float)m_app->window.getHeight();
    float aspect       = screenWidth / screenHeight;

    m_app->registry.emplace<Camera>(m_app->sceneManager.getCurrentSceneEntity(),
                                    Camera::ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                                    SCREEN_UNITS, 0.1f, 100.0f);
  }
  void generateSquares(size_t n) {
    const std::vector<float> vertices = {
        0.5f,  0.5f,  // top right
        0.5f,  -0.5f, // bottom right
        -0.5f, -0.5f, // bottom left
        -0.5f, 0.5f   // top left
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    // Setup random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> rand(0.f, 1.f);

    // Setup square properties
    RenderData renderData{2, vertices, indices};

    ShaderId shader = m_app->shaderManager.get("default");

    glm::vec3 startPos{0.f, 40.f, 0.f};
    glm::quat rotation{1.f, 0.f, 0.f, 0.f};
    glm::vec3 scale{5.f, 5.f, 5.f};

    for (size_t i = 0; i < n; ++i) {
      float xDisplacement = (rand(mt) - 0.5f) * 100.f;
      glm::vec3 pos = startPos + glm::vec3(xDisplacement, 0.f, 0.f);
      glm::vec4 color{rand(mt), rand(mt), rand(mt), 1.f};

      entt::entity entity = m_app->registry.create();
      m_app->registry.emplace<Transform>(entity, pos, rotation, scale);
      m_app->registry.emplace<Renderable>(entity, GL_TRIANGLES, renderData);
      m_app->registry.emplace<Material>(entity, color, shader);
      m_app->registry.emplace<Rigidbody>(entity, glm::vec3());
    }
  }
};