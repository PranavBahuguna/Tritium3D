#pragma once

#include <glm/glm.hpp>

#include <cstdint>

namespace RenderingBenchmark
{
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static bool USE_INSTANCED        = true;
  constexpr static float CONTAINER_SIZE      = 80.f;
  constexpr static glm::vec3 SHAPE_VELOCITY  = {10.f, 10.f, 0.f};
  constexpr static glm::vec3 START_POSITION  = {0.f, 0.f, 0.f};
  constexpr static float DISPLACEMENT_RADIUS = 10.f;
  constexpr static int NUM_SHAPES            = 10000;
} // namespace RenderingBenchmark
