#pragma once

#include <TritiumEngine/Rendering/Components/Color.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Utilities
{
  constexpr static uint32_t COLOR_RED     = 0xFF0000FF;
  constexpr static uint32_t COLOR_GREEN   = 0xFF00FF00;
  constexpr static uint32_t COLOR_BLUE    = 0xFFFF0000;
  constexpr static uint32_t COLOR_YELLOW  = 0xFF00FFFF;
  constexpr static uint32_t COLOR_MAGENTA = 0xFFFF00FF;
  constexpr static uint32_t COLOR_CYAN    = 0xFFFFFF00;
  constexpr static uint32_t COLOR_BLACK   = 0xFF000000;
  constexpr static uint32_t COLOR_WHITE   = 0xFFFFFFFF;

  constexpr static uint32_t COLOR_OPAQUE      = 0xFF000000;
  constexpr static uint32_t COLOR_TRANSPARENT = 0x00000000;
  constexpr static uint32_t COLOR_NONE        = 0x00000000;

  class ColorUtils {
  public:
    /**
     * @brief Converts uint32_t color value to an RGBA vec4
     */
    static glm::vec4 ToVec4(Color color) {
      return glm::vec4{
          color.value & 0x0000000FF,        // r
          (color.value & 0x0000FF00) >> 8,  // g
          (color.value & 0x00FF0000) >> 16, // b
          (color.value & 0xFF000000) >> 24  // a
      };
    }

    /**
     * @brief Converts uint32_t color value to a normalized RGBA vec4
     */
    static glm::vec4 ToNormalizedVec4(Color color) { return ToVec4(color) / 255.f; }

    /**
     * @brief Converts uint8_t RGBA color components to a uint32_t color value
     */
    static Color FromRGBAComponents(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
      return r + (g << 8) + (b << 16) + (a << 24);
    }
  };
} // namespace TritiumEngine::Utilities