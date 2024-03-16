#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  InstancedRenderable::InstancedRenderable(unsigned int renderMode, const RenderData &renderData,
                                           int count)
      : m_nInstances(count), m_vertexStride(renderData.vertexStride), m_renderMode(renderMode),
        m_ebo(0), m_instanceData(count) {

    Logger::info("[InstancedRenderable] Instance data size = {}",
                 m_nInstances * sizeof(InstanceData));

    static uint32_t _id = 0;
    m_instanceId        = _id++;

    const auto &vertices = renderData.vertices;
    const auto &indices  = renderData.indices;
    m_nVertices          = static_cast<int>(vertices.size());
    m_nIndices           = static_cast<int>(indices.size());

    // Bind vertex array objects for all instances
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Bind vertex data buffer
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, m_vertexStride, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Bind index data buffer
    if (m_nIndices > 0) {
      glGenBuffers(1, &m_ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(unsigned int), indices.data(),
                   GL_STATIC_DRAW);
    }

    // Bind instance data buffer
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ARRAY_BUFFER, m_nInstances * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW);

    // Instance models
    for (unsigned int i = 1; i < 5; ++i) {
      glEnableVertexAttribArray(i);
      glVertexAttribDivisor(i, 1);
      glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                            (void *)((i - 1) * sizeof(glm::vec4)));
    }

    // Instance colors
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);
    glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData),
                          (void *)(sizeof(glm::mat4)));
  }

  InstancedRenderable::~InstancedRenderable() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_ibo);
  }

  void InstancedRenderable::setInstanceData(size_t index, const InstanceData &data) {
    m_instanceData[index] = data;
  }

  void InstancedRenderable::resizeInstanceDataBuffer(size_t newSize) {
    m_instanceData.resize(newSize);
    glBufferData(GL_ARRAY_BUFFER, m_nInstances * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW);
  }

  void InstancedRenderable::updateInstanceDataBuffer() const {
    glNamedBufferSubData(m_ibo, 0, m_nInstances * sizeof(InstanceData), m_instanceData.data());
  }
} // namespace TritiumEngine::Rendering