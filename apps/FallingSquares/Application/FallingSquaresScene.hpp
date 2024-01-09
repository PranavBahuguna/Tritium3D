#pragma once

#include "Gravity.hpp"

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>

#include <glm/gtc/matrix_transform.hpp>

#define SCREEN_UNITS 100.0f

using namespace TritiumEngine::Core;

class FallingSquaresScene : public Scene {
public:
  FallingSquaresScene() : Scene("FallingSquares") {}

protected:
  void init() override {
    Gravity *grav = getSystem<Gravity>();
    //if (grav != nullptr)
    //  grav->g = 0.01f;
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

    RenderData renderData{2, vertices, indices};

    glm::vec4 color{1.f, 0.5f, 0.2f, 1.f};
    ShaderId shader = m_app->shaderManager.get("default");

    glm::vec3 currentPos{-40.f, 40.f, 0.f};
    glm::quat rotation{1.f, 0.f, 0.f, 0.f};
    glm::vec3 scale{5.f, 5.f, 5.f};

    for (size_t i = 0; i < n; ++i) {
      glm::vec3 pos = currentPos;
      currentPos.x += 10.f;

      entt::entity entity = m_app->registry.create();
      m_app->registry.emplace<Transform>(entity, currentPos, rotation, scale);
      m_app->registry.emplace<Renderable>(entity, GL_TRIANGLES, renderData);
      m_app->registry.emplace<Material>(entity, color, shader);
      m_app->registry.emplace<Rigidbody>(entity, glm::vec3());
    }
  }
};