#include <Tritium3D/Rendering/Renderables/Renderable.hpp>
#include <Tritium3D/Rendering/Renderer.hpp>

namespace Tritium3D::Rendering
{
  Renderer::Renderer() : m_prevShader(0) {}

  // Draws all supplied renderables with a given shader
  void Renderer::Render(const RenderList &renderList, const Shader &shader) {
    UseShader(shader);
    for (const auto &renderable : renderList)
      renderable->Draw();
  }

  // Clears screen
  void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  }

  // Activates the given shader if not already in use
  void Renderer::UseShader(const Shader &shader) {
    ShaderId newShader = shader.GetID();

    if (m_prevShader != newShader) {
      m_prevShader = newShader;
      shader.Use();
    }
  }
} // namespace Tritium3D::Rendering