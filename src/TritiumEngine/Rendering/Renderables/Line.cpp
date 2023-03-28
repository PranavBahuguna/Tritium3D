#include <TritiumEngine/Rendering/Renderables/Line.hpp>

namespace TritiumEngine::Rendering
{
  Line::Line(const glm::vec2 &start, const glm::vec2 &end) : Renderable() {
    std::vector<float> vertices = {start.x, start.y, end.x, end.y};

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Bind vertex data buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 2, vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
  }

  Line::~Line() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
  }

  void Line::Draw() const {
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 2);
  }
} // namespace TritiumEngine::Rendering