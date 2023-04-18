#include <TritiumEngine/Rendering/Renderable.hpp>

namespace TritiumEngine::Rendering
{
  Renderable::Renderable(GLenum renderMode, const RenderData &renderData)
      : m_renderMode(renderMode), m_EBO(0) {
    unsigned int stride = renderData.vertexStride;
    const auto &vertices = renderData.vertices;
    const auto &indices = renderData.indices;

    m_nVertices = (GLsizei)vertices.size();
    m_nIndices = (GLsizei)indices.size();

    // Bind vertex array object
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Bind vertex data buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Bind index data buffer
    if (m_nIndices > 0) {
      glGenBuffers(1, &m_EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),
                   GL_STATIC_DRAW);
    }
  }

  Renderable::~Renderable() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
  }

  GLuint Renderable::GetVAO() const { return m_VAO; }

  GLsizei Renderable::GetNumVertices() const { return m_nVertices; }

  GLsizei Renderable::GetNumIndices() const { return m_nIndices; }

  GLenum Renderable::GetRenderMode() const { return m_renderMode; }
} // namespace TritiumEngine::Rendering