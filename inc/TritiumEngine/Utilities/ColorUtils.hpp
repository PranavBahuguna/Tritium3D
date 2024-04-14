#pragma once

#include <TritiumEngine/Rendering/Components/Color.hpp>

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

  constexpr static uint32_t COLOR_OPAQUE      = 0xFF000000;
  constexpr static uint32_t COLOR_TRANSPARENT = 0x00000000;

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

    /**
     * @brief Obtains a color from a rainbow gradient using a ratio value
     * @param ratio A ratio value from 0 to 1 determining the color to obtain from gradient
     */
    static Color RainbowGradient(float ratio) {
      // we want to normalize ratio so that it fits in to 5 regions
      // where each region is 256 units long
      int normalized = int(ratio * 256 * 5);

      // find the region for this position
      int region = normalized / 256;

      // find the distance to the start of the closest region
      int x = normalized % 256;

      uint8_t r = 0, g = 0, b = 0;
      switch (region) {
      case 0:
        r = 255;
        g = x;
        b = 0;
        break;
      case 1:
        r = 255 - x;
        g = 255;
        b = 0;
        break;
      case 2:
        r = 0;
        g = 255;
        b = x;
        break;
      case 3:
        r = 0;
        g = 255 - x;
        b = 255;
        break;
      case 4:
        r = x;
        g = 0;
        b = 255;
        break;
      }
      return (r + (g << 8) + (b << 16)) | COLOR_OPAQUE;
    }
  };
} // namespace TritiumEngine::Utilities