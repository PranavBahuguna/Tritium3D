#pragma once

#include <TritiumEngine/Rendering/Color.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Utilities
{
  constexpr static uint32_t COLOR_RED    = 0xFF0000FF;
  constexpr static uint32_t COLOR_GREEN  = 0xFF00FF00;
  constexpr static uint32_t COLOR_BLUE   = 0xFFFF0000;
  constexpr static uint32_t COLOR_YELLOW = 0xFF00FFFF;
  constexpr static uint32_t COLOR_PINK   = 0xFFFF00FF;
  constexpr static uint32_t COLOR_CYAN   = 0xFFFFFF00;
  constexpr static uint32_t COLOR_BLACK  = 0xFF000000;
  constexpr static uint32_t COLOR_WHITE  = 0xFFFFFFFF;

  class ColorUtils {
  public:
    /**
     * @brief Converts uint32_t color value to a normalized vec4
     */
    static glm::vec4 ToNormalizedVec4(Color color) {
      return glm::vec4{
          (color.value & 0x0000000FF) / 255.f,        // r
          ((color.value & 0x0000FF00) >> 8) / 255.f,  // g
          ((color.value & 0x00FF0000) >> 16) / 255.f, // b
          ((color.value & 0xFF000000) >> 24) / 255.f  // a
      };
    }
  };
} // namespace TritiumEngine::Utilities