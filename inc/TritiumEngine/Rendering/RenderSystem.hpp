#pragma once

#include <TritiumEngine/Core/System.hpp>

#include <entt/entity/entity.hpp>

#include <vector>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class ShaderManager;
  struct Camera;

  class RenderSystem : public System {
  public:
    void update(float dt) override;

  private:
    void draw(const Camera &camera) const;
    void drawInstanced(const Camera &camera) const;
  };
} // namespace TritiumEngine::Rendering