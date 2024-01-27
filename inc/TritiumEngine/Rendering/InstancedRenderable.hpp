#ifndef INSTANCED_RERNDERABLE_HPP
#define INSTANCED_RERNDERABLE_HPP

#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/RenderData.hpp>

#include <GL/glew.h>

#include <vector>

using namespace TritiumEngine::Core;

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
    InstancedRenderable(GLenum renderMode, const RenderData &renderData, GLsizei count);
    virtual ~InstancedRenderable();

    void setInstanceData(size_t index, const InstanceData &data);
    void resizeInstanceDataBuffer(size_t newSize);
    void updateInstanceDataBuffer() const;

    GLuint getVao() const { return m_vao; }
    GLsizei getNumIndices() const { return m_nIndices; }
    GLsizei getNumVertices() const { return m_nVertices; }
    GLsizei getNumInstances() const { return m_nInstances; }
    GLenum getRenderMode() const { return m_renderMode; }
    uint32_t getInstanceId() const { return m_instanceId; }

  private:
    GLuint m_vao; // vertex array
    GLuint m_vbo; // vertex buffer
    GLuint m_ebo; // edges buffer
    GLuint m_ibo; // instance buffer

    GLsizei m_nInstances;
    GLsizei m_nVertices;
    GLsizei m_nIndices;
    GLenum m_renderMode;
    uint32_t m_instanceId;
    std::vector<InstanceData> m_instanceData;
  };
} // namespace TritiumEngine::Rendering

#endif // INSTANCED_RERNDERABLE_HPP