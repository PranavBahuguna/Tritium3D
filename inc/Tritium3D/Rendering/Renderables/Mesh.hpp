#pragma once

#include <Tritium3D/Rendering/Renderables/Renderable.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Tritium3D::Rendering
{
  class Mesh : public Renderable {
  public:
    Mesh(const std::vector<glm::vec2> &vertices, const std::vector<GLuint> &indices);
    ~Mesh();

    void Draw() const override;

  private:
    GLuint m_IBO;
    GLuint m_VAO;
    GLuint m_VBO;
    GLsizei m_nIndices;
  };
} // namespace Tritium3D::Rendering