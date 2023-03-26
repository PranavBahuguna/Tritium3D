#pragma once

#include <memory>
#include <vector>

namespace Tritium3D::Rendering
{
  class Renderable {
  public:
    virtual ~Renderable() {}

    virtual void Draw() const = 0;
  };

  typedef std::vector<std::shared_ptr<Renderable>> RenderList;
} // namespace Tritium3D::Rendering