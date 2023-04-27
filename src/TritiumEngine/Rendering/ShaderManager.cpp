#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>

#include <glm/gtc/type_ptr.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  ShaderManager::~ShaderManager() {
    // Delete all stored programs
    for (auto &item : m_nameToIdMap)
      glDeleteProgram(item.second);
  }

  /**
   * Creates a new shader program using vertex and fragment shader program's code. Will overwrite
   * any cached shader with the same name.
   *
   * @param name The name of the new shader to create.
   * @param vertexData The code string for the vertex shader program.
   * @param fragmentData The code string for the fragment shader program.
   * @returns Id of the new shader program.
   */
  ShaderId ShaderManager::Create(const std::string &name, const std::string &vertexData,
                                 const std::string &fragmentData) {
    // Compile vertex and fragment shaders
    ShaderId vertexId   = Compile(vertexData.c_str(), GL_VERTEX_SHADER);
    ShaderId fragmentId = Compile(fragmentData.c_str(), GL_FRAGMENT_SHADER);
    // Link the shaders
    ShaderId programId = Link(vertexId, fragmentId);

    // Add shader program to storage
    if (programId != 0)
      m_nameToIdMap[name] = programId;

    return programId;
  }

  /**
   * Loads a shader from file or cache using its base name.
   *
   * @param name The base name of the shader to load. The constituent vertex/fragment shaders are
   * loaded using the base name + .vert/frag extensions.
   * @param forceReload If true, will force a reload of the vertex/fragment files and overwrite any
   * existing shader of the same name.
   * @returns Id of the new shader program.
   */
  ShaderId ShaderManager::Get(const std::string &name, bool forceReload) {
    if (!forceReload) {
      // Check if the shader program has already been loaded
      auto it = m_nameToIdMap.find(name);
      if (it != m_nameToIdMap.end())
        return it->second;
    }

    const auto &vertexData   = ResourceManager<ShaderCode>::Get(name + ".vert", forceReload);
    const auto &fragmentData = ResourceManager<ShaderCode>::Get(name + ".frag", forceReload);

    if (vertexData == nullptr || fragmentData == nullptr)
      return 0;

    return Create(name, vertexData->data, fragmentData->data);
  }

  /**
   * Activates this shader program.
   *
   * @param id The id of the shader program to activate.
   */
  void ShaderManager::Use(ShaderId id) {
    if (id != m_currentShaderId) {
      glUseProgram(id);
      m_currentShaderId = id;
    }
  }

  void ShaderManager::SetBool(const std::string &name, bool value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void ShaderManager::SetInt(const std::string &name, int value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void ShaderManager::SetUint(const std::string &name, unsigned int value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1ui(uniformLocation, value);
  }

  void ShaderManager::SetFloat(const std::string &name, float value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1f(uniformLocation, value);
  }

  void ShaderManager::SetVector2(const std::string &name, const glm::vec2 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::SetVector2(const std::string &name, GLfloat x, GLfloat y) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform2f(uniformLocation, x, y);
  }

  void ShaderManager::SetVector3(const std::string &name, const glm::vec3 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::SetVector3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform3f(uniformLocation, x, y, z);
  }

  void ShaderManager::SetVector4(const std::string &name, const glm::vec4 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::SetVector4(const std::string &name, GLfloat x, GLfloat y, GLfloat z,
                                 GLfloat w) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform4f(uniformLocation, x, y, z, w);
  }

  void ShaderManager::SetMatrix2(const std::string &name, const glm::mat2 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void ShaderManager::SetMatrix3(const std::string &name, const glm::mat3 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void ShaderManager::SetMatrix4(const std::string &name, const glm::mat4 &value) {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  // Compiles shader from code, returning its program ID
  ShaderId ShaderManager::Compile(const char *shaderCode, GLenum shaderType) {
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
      Logger::Log(LogType::ERROR, "An error occurred while compiling shader:\n{}", errorLog);
    }

    return shaderId;
  }

  // Links the constituent vertex and fragment shader programs to produce the complete shader
  // program
  ShaderId ShaderManager::Link(ShaderId vertexId, ShaderId fragmentId) {
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
      Logger::Log(LogType::ERROR, "An error occurred while linking shader:\n{}", errorLog);
    }

    return program;
  }
} // namespace TritiumEngine::Rendering