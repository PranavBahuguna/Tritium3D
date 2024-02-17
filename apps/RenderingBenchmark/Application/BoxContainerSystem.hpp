#pragma once

#include "Rigidbody.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Color.hpp>

using namespace TritiumEngine::Core;

class BoxContainerSystem : public System {
public:
  BoxContainerSystem(float boxSize = 100.f) : System(), m_boxSize(boxSize) {}

  void update(float dt) override {
    m_app->registry.view<Rigidbody, Transform, Color>().each(
        [&](auto entity, Rigidbody &rigidbody, Transform &transform, Color &color) {
          bool hasCollided  = false;
          float halfBoxSize = m_boxSize / 2.f;
          auto nextPos      = transform.getPosition() + rigidbody.velocity * dt;

          if (nextPos.x > halfBoxSize) {
            rigidbody.velocity.x *= -1;
            float diff = nextPos.x - halfBoxSize;
            nextPos    = {halfBoxSize - diff, nextPos.y, nextPos.z};
            updateColor(rigidbody.velocity, color);
          }
          
          if (nextPos.x < -halfBoxSize) {
            rigidbody.velocity.x *= -1;
            float diff = nextPos.x + halfBoxSize;
            nextPos    = {-halfBoxSize - diff, nextPos.y, nextPos.z};
            updateColor(rigidbody.velocity, color);
          }

          if (nextPos.y > halfBoxSize) {
            rigidbody.velocity.y *= -1;
            float diff = nextPos.y - halfBoxSize;
            nextPos    = {nextPos.x, halfBoxSize - diff, nextPos.z};
            updateColor(rigidbody.velocity, color);
          }
          
          if (nextPos.y < -halfBoxSize) {
            rigidbody.velocity.y *= -1;
            float diff = nextPos.y + halfBoxSize;
            nextPos    = {nextPos.x, -halfBoxSize - diff, nextPos.z};
            updateColor(rigidbody.velocity, color);
          }

          transform.setPosition(nextPos);
        });
  }

private:
  void updateColor(glm::vec3 &velocity, Color &color) {
    bool posVelocityX = velocity.x > 0;
    bool posVelocityY = velocity.y > 0;

    if (posVelocityX && posVelocityY)
      color = Color{0xFF0000FF}; // RED
    if (posVelocityX && !posVelocityY)
      color = Color{0xFF00FF00}; // GREEN
    if (!posVelocityX && posVelocityY)
      color = Color{0xFFFF0000}; // BLUE
    if (!posVelocityX && !posVelocityY)
      color = Color{0xFF00FFFF}; // YELLOW
  }

  float m_boxSize;
};