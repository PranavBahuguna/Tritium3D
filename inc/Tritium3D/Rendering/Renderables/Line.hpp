#pragma once

#include <Tritium3D/Rendering/Renderables/Renderable.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Tritium3D::Rendering
{
  class Line : public Renderable {
  public:
    Line(const glm::vec2 &start, const glm::vec2 &end);
    ~Line();

    void Draw() const override;

  private:
    GLuint m_VAO;
    GLuint m_VBO;
  };
} // namespace Tritium3D::Rendering