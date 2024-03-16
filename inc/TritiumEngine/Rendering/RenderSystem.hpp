#pragma once

#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Core/Tag.hpp>
#include <TritiumEngine/Rendering/BlendOptions.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera;

  class RenderSystem : public System {
  public:
    RenderSystem(Tag cameraTag, BlendOptions blendOptions);

    void update(float dt) override;
    virtual void draw(const Camera &camera) const = 0;

    void setCameraTag(Tag tag) { m_cameraTag = tag; }

  private:
    Tag m_cameraTag;
    BlendOptions m_blendOptions;
  };
} // namespace TritiumEngine::Rendering