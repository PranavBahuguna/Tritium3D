#pragma once

#include <glm/glm.hpp>

#include <cstdint>

namespace TritiumEngine::Rendering
{
  struct Color {
    Color(uint32_t value) : value(value) {}

    glm::vec4 asNormalizedVec4() {
      float r = (value & 0x0000000FF) / 255.f;
      float g = ((value & 0x0000FF00) >> 8) / 255.f;
      float b = ((value & 0x00FF0000) >> 16) / 255.f;
      float a = ((value & 0xFF000000) >> 24) / 255.f;
      return glm::vec4{r, g, b, a};
    }

    uint32_t value; // 0xRRGGBBAA
  };
} // namespace TritiumEngine::Rendering