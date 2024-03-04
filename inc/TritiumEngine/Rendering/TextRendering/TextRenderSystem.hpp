#pragma once

#include <TritiumEngine/Core/System.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering::TextRendering
{
  class Text;

  class TextRenderSystem : public System {
  public:
    void update(float dt) override;

  private:
    glm::vec2 getStartPosition(const Text &text) const;
  };
} // namespace TritiumEngine::Rendering::TextRendering