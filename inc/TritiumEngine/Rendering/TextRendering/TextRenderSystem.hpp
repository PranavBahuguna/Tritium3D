#pragma once

#include <TritiumEngine/Rendering/RenderSystem.hpp>

#include <glm/glm.hpp>

namespace TritiumEngine::Rendering::TextRendering
{
  class Text;

  class TextRenderSystem : public RenderSystem {
  public:
    TextRenderSystem(Tag cameraTag, BlendOptions blendOptions = {});

    void draw(const Camera &camera) const override;

  private:
    glm::vec2 getStartPosition(const Text &text) const;
  };
} // namespace TritiumEngine::Rendering::TextRendering