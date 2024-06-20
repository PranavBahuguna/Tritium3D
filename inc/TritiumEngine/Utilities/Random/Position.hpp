#pragma once

#include <TritiumEngine/Utilities/Random/Random.hpp>

#include <glm/glm.hpp>

#include <numbers>

using namespace TritiumEngine::Utilities::Random::Internal;

namespace TritiumEngine::Utilities::Random
{
  static glm::vec3 RadialPosition(float radius, bool uniform = true) {
    float a = uniformDist(mt);
    float b = uniformDist(mt);

    if (uniform && a > b)
      std::swap(a, b);

    float pi    = static_cast<float>(std::numbers::pi);
    float randX = b * radius * std::cos(2 * pi * a / b);
    float randY = b * radius * std::sin(2 * pi * a / b);

    return {randX, randY, 0.f};
  }

  static glm::vec3 CubePosition(float size) {
    float x = size * (uniformDist(mt) - 0.5f);
    float y = size * (uniformDist(mt) - 0.5f);
    float z = size * (uniformDist(mt) - 0.5f);

    return {x, y, z};
  }

  static glm::vec3 Velocity2D(float v) {
    float vX = uniformDist(mt) * v;
    float vY = sqrtf(v * v - vX * vX);

    return {vX, vY, 0.f};
  }
} // namespace TritiumEngine::Utilities::Random