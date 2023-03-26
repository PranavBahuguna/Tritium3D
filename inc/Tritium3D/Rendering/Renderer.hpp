#pragma once

#include <Tritium3D/Rendering/Renderables/Renderable.hpp>
#include <Tritium3D/Rendering/Shader.hpp>

namespace Tritium3D::Rendering
{
  class Renderer {
  public:
    Renderer();

    void Render(const RenderList &renderList, const Shader &shader);
    void Clear();

  private:
    void UseShader(const Shader &shader);

    // This allows the renderer to detect if we are still using the previous shader program. If so,
    // it can optimise the code by avoid reassigning the same shader program repeatedly.
    ShaderId m_prevShader;
  };
} // namespace Tritium3D::Rendering