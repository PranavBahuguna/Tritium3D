#pragma once

#include <TritiumEngine/Entities/World.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>

using namespace TritiumEngine::Entities;

namespace TritiumEngine::Rendering
{
  class RenderSystem {
  public:
    void Draw(const Camera &camera, const World &world) const;
    void DrawInstanced(const Camera &camera, const World &world) const;
  };
} // namespace TritiumEngine::Rendering