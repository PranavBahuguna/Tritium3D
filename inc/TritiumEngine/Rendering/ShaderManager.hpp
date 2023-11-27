#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

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
                    const std::string &fragmentData);
    ShaderId get(const std::string &name, bool forceReload = false);
    void Use(ShaderId id);

    // Shader parameter setter methods
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setUint(const std::string &name, unsigned int value);
    void setFloat(const std::string &name, float value);
    void setVector2(const std::string &name, const glm::vec2 &value);
    void setVector2(const std::string &name, GLfloat x, GLfloat y);
    void setVector3(const std::string &name, const glm::vec3 &value);
    void setVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
    void setVector4(const std::string &name, const glm::vec4 &value);
    void setVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void setMatrix2(const std::string &name, const glm::mat2 &value);
    void setMatrix3(const std::string &name, const glm::mat3 &value);
    void setMatrix4(const std::string &name, const glm::mat4 &value);

  private:
    ShaderId compile(const char *shaderCode, GLenum shaderType);
    ShaderId link(ShaderId vertexId, ShaderId fragmentId);

    std::unordered_map<std::string, ShaderId> m_nameToIdMap;
    ShaderId m_currentShaderId = 0;
  };
} // namespace TritiumEngine::Rendering

#endif // SHADER_MANAGER_HPP