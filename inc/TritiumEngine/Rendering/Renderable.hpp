#pragma once

#include <TritiumEngine/Rendering/RenderData.hpp>

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  class Renderable {
  public:
    Renderable(GLenum renderMode, const RenderData &renderData);
    virtual ~Renderable();

    GLuint getVao() const { return m_vao; }
    GLint getVertexStride() const { return m_vertexStride; }
    GLsizei getNumVertices() const { return m_nVertices; }
    GLsizei getNumIndices() const { return m_nIndices; }
    GLenum getRenderMode() const { return m_renderMode; }

  private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLint m_vertexStride;
    GLsizei m_nVertices;
    GLsizei m_nIndices;
    GLenum m_renderMode;
  };
} // namespace TritiumEngine::Rendering