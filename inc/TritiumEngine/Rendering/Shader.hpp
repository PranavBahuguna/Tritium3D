#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

namespace TritiumEngine::Rendering
{
  typedef unsigned int ShaderId;

  class Shader {
  public:
    Shader(ShaderId id);
    Shader(const std::string &vertexShaderCode, const std::string &fragmentShaderCode);
    ~Shader();

    void Use() const;
    ShaderId GetID() const;

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetUint(const std::string &name, unsigned int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVector2(const std::string &name, const glm::vec2 &value) const;
    void SetVector2(const std::string &name, GLfloat x, GLfloat y) const;
    void SetVector3(const std::string &name, const glm::vec3 &value) const;
    void SetVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
    void SetVector4(const std::string &name, const glm::vec4 &value) const;
    void SetVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
    void SetMatrix2(const std::string &name, const glm::mat2 &value) const;
    void SetMatrix3(const std::string &name, const glm::mat3 &value) const;
    void SetMatrix4(const std::string &name, const glm::mat4 &value) const;

  private:
    ShaderId Compile(const char *shaderCode, GLenum shaderType) const;
    ShaderId Link(ShaderId vertexId, ShaderId fragmentId) const;

    ShaderId m_id;
  };
} // namespace TritiumEngine::Rendering