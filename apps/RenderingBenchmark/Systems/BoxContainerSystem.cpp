#pragma once

#include "Systems/BoxContainerSystem.hpp"
#include "Components/Rigidbody.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Utilities;
using namespace RenderingBenchmark::Components;

namespace RenderingBenchmark::Systems
{
  BoxContainerSystem::BoxContainerSystem(float boxSize) : System(), m_boxSize(boxSize) {}

  void BoxContainerSystem::update(float dt) {
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

  void BoxContainerSystem::updateColor(glm::vec3 &velocity, Color &color) {
    bool posVelocityX = velocity.x > 0;
    bool posVelocityY = velocity.y > 0;

    if (posVelocityX && posVelocityY)
      color = COLOR_RED;
    if (posVelocityX && !posVelocityY)
      color = COLOR_GREEN;
    if (!posVelocityX && posVelocityY)
      color = COLOR_BLUE;
    if (!posVelocityX && !posVelocityY)
      color = COLOR_YELLOW;
  }
} // namespace RenderingBenchmark::Systems