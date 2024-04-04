#pragma once

#include <glm/glm.hpp>

namespace TritiumEngine::Rendering
{
  struct Font {
    constexpr static size_t CHAR_ARRAY_SIZE = 128;

    struct Character {
      unsigned int textureID; // ID handle of the glyph texture
      glm::ivec2 size;        // Size of glyph
      glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
      signed long advance;    // Offset to advance to next glyph
    };

    Character characters[CHAR_ARRAY_SIZE];
  };
} // namespace TritiumEngine::Rendering