#pragma once

#include <glm/glm.hpp>

#include <cstdint>

namespace TritiumEngine::Rendering
{
  struct Color {
    Color(uint32_t value = 0) : value(value) {}

    uint32_t value; // 0xAABBGGRR
  };
} // namespace TritiumEngine::Rendering