#pragma once

#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Rendering/BlendOptions.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera;

  class RenderSystem : public System {
  public:
    RenderSystem(BlendOptions blendOptions);

    void update(float dt) override;
    virtual void draw(const Camera &camera) const = 0;

  private:
    BlendOptions m_blendOptions;
  };
} // namespace TritiumEngine::Rendering