#pragma once

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  struct RenderSettings {
    bool enableDepthTest      = false;
    unsigned int depthFunc    = GL_LESS;
    bool enableBlend          = false;
    unsigned int blendSFactor = GL_ZERO;
    unsigned int blendDFactor = GL_ONE;

    void apply() const {
      // Apply depth test
      bool depthPrevEnabled = glIsEnabled(GL_DEPTH_TEST);
      if (enableDepthTest) {
        glDepthFunc(depthFunc);
        if (!depthPrevEnabled)
          glEnable(GL_DEPTH_TEST);
      } else if (depthPrevEnabled) {
        glDisable(GL_DEPTH_TEST);
      }

      // Apply blend
      bool blendPrevEnabled = glIsEnabled(GL_BLEND);
      if (enableBlend) {
        glBlendFunc(blendSFactor, blendDFactor);
        if (!blendPrevEnabled)
          glEnable(GL_BLEND);
      } else if (blendPrevEnabled) {
        glDisable(GL_BLEND);
      }
    }
  };
} // namespace TritiumEngine::Rendering