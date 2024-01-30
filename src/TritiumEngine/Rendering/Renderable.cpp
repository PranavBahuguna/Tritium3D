#include <TritiumEngine/Rendering/Renderable.hpp>

namespace TritiumEngine::Rendering
{
  Renderable::Renderable(GLenum renderMode, const RenderData &renderData)
      : m_renderMode(renderMode), m_ebo(0) {
    unsigned int vertexStride = renderData.vertexStride;
    const auto &vertices      = renderData.vertices;
    const auto &indices       = renderData.indices;

    m_vertexStride = static_cast<GLint>(vertexStride);
    m_nVertices    = static_cast<GLsizei>(vertices.size());
    m_nIndices     = static_cast<GLsizei>(indices.size());

    // Bind vertex array object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Bind vertex data buffer
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, m_vertexStride, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // Bind index data buffer
    if (m_nIndices > 0) {
      glGenBuffers(1, &m_ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(GLuint), indices.data(),
                   GL_STATIC_DRAW);
    }
  }

  Renderable::~Renderable() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
  }
} // namespace TritiumEngine::Rendering