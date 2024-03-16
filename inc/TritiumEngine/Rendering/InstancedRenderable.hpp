#pragma once

#include <TritiumEngine/Rendering/RenderData.hpp>

#include <glm/glm.hpp>

namespace TritiumEngine::Rendering
{
  struct InstancedRenderableTag {
    uint32_t value;
  };

  struct InstanceData {
    glm::mat4 model;
    uint32_t color;
  };

  class InstancedRenderable {
  public:
    InstancedRenderable(unsigned int renderMode, const RenderData &renderData, int count);
    virtual ~InstancedRenderable();

    void setInstanceData(size_t index, const InstanceData &data);
    void resizeInstanceDataBuffer(size_t newSize);
    void updateInstanceDataBuffer() const;

    unsigned int getVao() const { return m_vao; }
    int getVertexStride() const { return m_vertexStride; }
    int getNumVertices() const { return m_nVertices; }
    int getNumIndices() const { return m_nIndices; }
    int getNumInstances() const { return m_nInstances; }
    unsigned int getRenderMode() const { return m_renderMode; }
    uint32_t getInstanceId() const { return m_instanceId; }

  private:
    unsigned int m_vao; // vertex array
    unsigned int m_vbo; // vertex buffer
    unsigned int m_ebo; // edges buffer
    unsigned int m_ibo; // instance data buffer

    int m_nInstances;
    int m_vertexStride;
    int m_nVertices;
    int m_nIndices;
    unsigned int m_renderMode;
    uint32_t m_instanceId;
    std::vector<InstanceData> m_instanceData;
  };
} // namespace TritiumEngine::Rendering