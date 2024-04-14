#pragma once

#include <glm/glm.hpp>

#include <numbers>
#include <random>

namespace TritiumEngine::Utilities
{
  namespace
  {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<float> rand(0.f, 1.f);
  } // namespace

  struct RandomUtils {
    static glm::vec3 RadialPosition(float radius, bool uniform = true) {
      float a = rand(mt);
      float b = rand(mt);

      if (uniform && a > b)
        std::swap(a, b);

      float pi    = static_cast<float>(std::numbers::pi);
      float randX = b * radius * std::cos(2 * pi * a / b);
      float randY = b * radius * std::sin(2 * pi * a / b);

      return {randX, randY, 0.f};
    }

    static glm::vec3 CubePosition(float size) {
      float x = size * (rand(mt) - 0.5f);
      float y = size * (rand(mt) - 0.5f);
      float z = size * (rand(mt) - 0.5f);

      return {x, y, z};
    }
  };
} // namespace TritiumEngine::Utilities