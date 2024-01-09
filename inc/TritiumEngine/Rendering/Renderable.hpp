#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <TritiumEngine/Rendering/RenderData.hpp>

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  class Renderable {
  public:
    Renderable(GLenum renderMode, const RenderData &renderData);
    virtual ~Renderable();

    GLuint getVAO() const;
    GLsizei getNumVertices() const;
    GLsizei getNumIndices() const;
    GLenum getRenderMode() const;

  private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLsizei m_nVertices;
    GLsizei m_nIndices;
    GLenum m_renderMode;
  };
} // namespace TritiumEngine::Rendering

#endif // RENDERABLE_HPP