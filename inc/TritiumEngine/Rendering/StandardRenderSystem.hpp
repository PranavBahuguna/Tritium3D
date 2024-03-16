#pragma once

#include <TritiumEngine/Rendering/RenderSystem.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class StandardRenderSystem : public RenderSystem {
  public:
    StandardRenderSystem(Tag cameraTag, BlendOptions blendOptions = {});

    void draw(const Camera &camera) const override;

  private:
    void drawNonInstanced(const Camera &camera) const;
    void drawInstanced(const Camera &camera) const;
  };
} // namespace TritiumEngine::Rendering