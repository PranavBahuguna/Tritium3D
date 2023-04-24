#pragma once

#include <TritiumEngine/Entities/World.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>

using namespace TritiumEngine::Entities;

namespace TritiumEngine::Rendering
{
  class RenderSystem {
  public:
    RenderSystem(const std::shared_ptr<ShaderManager> shaderManager);

    void Draw(const Camera &camera, const World &world) const;
    void DrawInstanced(const Camera &camera, const World &world) const;

  private:
    std::shared_ptr<ShaderManager> m_shaderManager;
  };
} // namespace TritiumEngine::Rendering