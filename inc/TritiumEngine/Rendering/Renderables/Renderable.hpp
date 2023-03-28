#pragma once

#include <memory>
#include <vector>

namespace TritiumEngine::Rendering
{
  class Renderable {
  public:
    virtual ~Renderable() {}

    /**
     * Implements process of drawing the renderable to the screen.
     */
    virtual void Draw() const = 0;
  };

  typedef std::vector<std::shared_ptr<Renderable>> RenderList;
} // namespace TritiumEngine::Rendering