#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <TritiumEngine/Rendering/RenderData.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace TritiumEngine::Rendering
{
  class Renderable {
  public:
    Renderable(GLenum renderMode, const RenderData &renderData);
    virtual ~Renderable();

    GLuint GetVAO() const;
    GLsizei GetNumVertices() const;
    GLsizei GetNumIndices() const;
    GLenum GetRenderMode() const;

  private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLsizei m_nVertices;
    GLsizei m_nIndices;
    GLenum m_renderMode;
  };
} // namespace TritiumEngine::Rendering

#endif // RENDERABLE_HPP