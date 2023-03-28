#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

namespace TritiumEngine::Rendering
{
  typedef unsigned int ShaderId;

  class Shader {
  public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void Use() const;
    void SetMatrix4(const std::string &name, const glm::mat4 &matrix4) const;
    ShaderId GetID() const;

  private:
    std::string ReadFile(const std::string &filePath) const;
    ShaderId Compile(const char *shaderCode, GLenum shaderType) const;
    ShaderId Link(ShaderId vertexId, ShaderId fragmentId) const;

    ShaderId m_id;
  };
} // namespace TritiumEngine::Rendering