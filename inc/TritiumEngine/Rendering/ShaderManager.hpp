#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace TritiumEngine::Rendering
{
  typedef unsigned int ShaderId;

  class ShaderManager {
  public:
    ~ShaderManager();

    ShaderId create(const std::string &name, const std::string &vertexData,
                    const std::string &fragmentData, const std::string &geometryData);
    ShaderId get(const std::string &name, bool forceReload = false);
    void use(ShaderId id);

    // Shader parameter setter methods
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setUint(const std::string &name, unsigned int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVector2(const std::string &name, const glm::vec2 &value) const;
    void setVector2(const std::string &name, GLfloat x, GLfloat y) const;
    void setVector3(const std::string &name, const glm::vec3 &value) const;
    void setVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
    void setVector4(const std::string &name, const glm::vec4 &value) const;
    void setVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
    void setMatrix2(const std::string &name, const glm::mat2 &value) const;
    void setMatrix3(const std::string &name, const glm::mat3 &value) const;
    void setMatrix4(const std::string &name, const glm::mat4 &value) const;

  private:
    ShaderId compile(const char *shaderCode, GLenum shaderType);
    ShaderId link(const std::vector<ShaderId> &shaderPrograms);

    std::unordered_map<std::string, ShaderId> m_nameToIdMap;
    ShaderId m_currentShaderId = 0;
  };
} // namespace TritiumEngine::Rendering