#include <Tritium3D/Rendering/Renderables/Mesh.hpp>

namespace Tritium3D::Rendering
{
  Mesh::Mesh(const std::vector<glm::vec2> &vertices, const std::vector<GLuint> &indices)
      : Renderable(), m_nIndices((GLsizei)indices.size()) {
    // Bind vertex array object
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Bind vertex data buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 2, vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Bind index data buffer
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),
                 GL_STATIC_DRAW);
  }

  Mesh::~Mesh() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
  }

  void Mesh::Draw() const {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, 0);
  }
} // namespace Tritium3D::Rendering