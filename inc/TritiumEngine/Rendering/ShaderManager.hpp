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

    ShaderId Create(const std::string &name, const std::string &vertexData,
                           const std::string &fragmentData);
    ShaderId Get(const std::string &name, bool forceReload = false);

    void Use(ShaderId id);

    void SetBool(const std::string &name, bool value);
    void SetInt(const std::string &name, int value);
    void SetUint(const std::string &name, unsigned int value);
    void SetFloat(const std::string &name, float value);
    void SetVector2(const std::string &name, const glm::vec2 &value);
    void SetVector2(const std::string &name, GLfloat x, GLfloat y);
    void SetVector3(const std::string &name, const glm::vec3 &value);
    void SetVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
    void SetVector4(const std::string &name, const glm::vec4 &value);
    void SetVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void SetMatrix2(const std::string &name, const glm::mat2 &value);
    void SetMatrix3(const std::string &name, const glm::mat3 &value);
    void SetMatrix4(const std::string &name, const glm::mat4 &value);

  private:
    ShaderId Compile(const char *shaderCode, GLenum shaderType);
    ShaderId Link(ShaderId vertexId, ShaderId fragmentId);

    std::unordered_map<std::string, ShaderId> m_nameToIdMap;
    ShaderId m_currentShaderId = 0;
  };
} // namespace TritiumEngine::Rendering

#endif // SHADER_MANAGER_HPP