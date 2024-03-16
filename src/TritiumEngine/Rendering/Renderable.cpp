#include <TritiumEngine/Rendering/Renderable.hpp>

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  Renderable::Renderable(unsigned int renderMode, const RenderData &renderData)
      : m_vertexStride(renderData.vertexStride), m_renderMode(renderMode), m_ebo(0) {
    const auto &vertices = renderData.vertices;
    const auto &indices  = renderData.indices;
    m_nVertices          = static_cast<int>(vertices.size());
    m_nIndices           = static_cast<int>(indices.size());

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
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(unsigned int), indices.data(),
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