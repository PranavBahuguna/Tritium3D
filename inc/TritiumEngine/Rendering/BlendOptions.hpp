#pragma once

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  struct BlendOptions {
    bool enabled;
    unsigned int sFactor;
    unsigned int dFactor;

    void apply() const {
      if (enabled) {
        glEnable(GL_BLEND);
        glBlendFunc(sFactor, dFactor);
      } else {
        glDisable(GL_BLEND);
      }
    }
  };
} // namespace TritiumEngine::Rendering