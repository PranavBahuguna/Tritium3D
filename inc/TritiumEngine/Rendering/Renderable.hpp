#pragma once

#include <TritiumEngine/Rendering/RenderData.hpp>

namespace TritiumEngine::Rendering
{
  class Renderable {
  public:
    Renderable(unsigned int renderMode, const RenderData &renderData);
    virtual ~Renderable();

    unsigned int getVao() const { return m_vao; }
    int getVertexStride() const { return m_vertexStride; }
    int getNumVertices() const { return m_nVertices; }
    int getNumIndices() const { return m_nIndices; }
    unsigned int getRenderMode() const { return m_renderMode; }

  private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    int m_vertexStride;
    int m_nVertices;
    int m_nIndices;
    unsigned int m_renderMode;
  };
} // namespace TritiumEngine::Rendering