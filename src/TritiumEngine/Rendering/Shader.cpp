#include <TritiumEngine/Rendering/Shader.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace TritiumEngine::Rendering
{
  Shader::Shader(ShaderId id) : m_id(id) {}

  Shader::Shader(const std::string &vertexShaderCode, const std::string &fragmentShaderCode) {
    // Compile vertex and fragment shaders
    ShaderId vertexId = Compile(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    ShaderId fragmentId = Compile(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

    // Link the shaders
    m_id = Link(vertexId, fragmentId);
  }

  Shader::~Shader() { glDeleteProgram(m_id); }

  /** Activates this shader program */
  void Shader::Use() const { glUseProgram(m_id); }

  ShaderId Shader::GetID() const { return m_id; }

  void Shader::SetBool(const std::string &name, bool value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void Shader::SetInt(const std::string &name, int value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void Shader::SetUint(const std::string &name, unsigned int value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform1ui(uniformLocation, value);
  }

  void Shader::SetFloat(const std::string &name, float value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(uniformLocation, value);
  }

  void Shader::SetVector2(const std::string &name, const glm::vec2 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void Shader::SetVector2(const std::string &name, GLfloat x, GLfloat y) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform2f(uniformLocation, x, y);
  }

  void Shader::SetVector3(const std::string &name, const glm::vec3 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void Shader::SetVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform3f(uniformLocation, x, y, z);
  }

  void Shader::SetVector4(const std::string &name, const glm::vec4 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void Shader::SetVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z,
                          GLfloat w) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(uniformLocation, x, y, z, w);
  }

  void Shader::SetMatrix2(const std::string &name, const glm::mat2 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void Shader::SetMatrix3(const std::string &name, const glm::mat3 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void Shader::SetMatrix4(const std::string &name, const glm::mat4 &value) const {
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  // Compiles shader from code, returning its program ID
  ShaderId Shader::Compile(const char *shaderCode, GLenum shaderType) const {
    ShaderId shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);

    // Validate shader compilation
    int isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
      int infoLogLength = 0;
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
      char *errorLog = new char[infoLogLength];
      glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, &errorLog[0]);
      printf("An error occurred while compiling shader\n%s\n", errorLog);
    }

    return shaderId;
  }

  // Links the constituent vertex and fragment shader programs to produce the complete shader
  // program
  ShaderId Shader::Link(ShaderId vertexId, ShaderId fragmentId) const {
    ShaderId program = glCreateProgram();
    glAttachShader(program, vertexId);
    glAttachShader(program, fragmentId);
    glLinkProgram(program);

    // Validate shader linking
    int isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

    if (!isLinked) {
      int infoLogLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
      char *errorLog = new char[infoLogLength];
      glGetProgramInfoLog(program, infoLogLength, &infoLogLength, &errorLog[0]);
      printf("An error occurred while linking shader:\n%s\n", errorLog);
    }

    return program;
  }
} // namespace TritiumEngine::Rendering