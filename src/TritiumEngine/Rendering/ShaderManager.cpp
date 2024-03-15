#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderCode.hpp>
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
   * @brief Creates a new shader program using supplied program's code. Will overwrite any cached
   * shader with the same name.
   * @param name The name of the new shader to create
   * @param vertexData The code string for the vertex shader program
   * @param fragmentData The code string for the fragment shader program
   * @param geometryData The code string for the geometry shader program, optional
   * @returns Id of the new shader program
   */
  ShaderId ShaderManager::create(const std::string &name, const std::string &vertexData,
                                 const std::string &fragmentData,
                                 const std::string &geometryData = "") {
    ShaderId programId = 0;
    // Compile vertex and fragment shaders
    ShaderId vertexId   = compile(vertexData.c_str(), GL_VERTEX_SHADER);
    ShaderId fragmentId = compile(fragmentData.c_str(), GL_FRAGMENT_SHADER);
    if (!geometryData.empty()) {
      ShaderId geometryId = compile(geometryData.c_str(), GL_GEOMETRY_SHADER);
      programId           = link({vertexId, fragmentId, geometryId});
    } else {
      programId = link({vertexId, fragmentId});
    }

    // Add shader program to storage
    if (programId != 0)
      m_nameToIdMap[name] = programId;

    return programId;
  }

  /**
   * @brief Loads a shader from file or cache using its base name
   * @param name The base name of the shader to load. The constituent shaders are loaded using the
   * base name + a specific extension.
   * @param forceReload If true, will force a reload of the vertex/fragment files and overwrite any
   * existing shader of the same name
   * @returns Id of the new shader program
   */
  ShaderId ShaderManager::get(const std::string &name, bool forceReload) {
    if (!forceReload) {
      // Check if the shader program has already been loaded
      auto it = m_nameToIdMap.find(name);
      if (it != m_nameToIdMap.end())
        return it->second;
    }

    const auto &vertexShader   = ResourceManager<ShaderCode>::get(name + ".vert", forceReload);
    const auto &fragmentShader = ResourceManager<ShaderCode>::get(name + ".frag", forceReload);

    if (vertexShader == nullptr || fragmentShader == nullptr)
      return 0;

    if (!ResourceManager<ShaderCode>::fileExists(name + ".geom"))
      return create(name, vertexShader->data, fragmentShader->data);

    // Add the geometry shader if one can be found
    const auto &geometryShader = ResourceManager<ShaderCode>::get(name + ".geom", forceReload);
    if (geometryShader == nullptr)
      return 0;

    return create(name, vertexShader->data, fragmentShader->data, geometryShader->data);
  }

  /**
   * @brief Activates this shader program.
   * @param id The id of the shader program to activate.
   */
  void ShaderManager::use(ShaderId id) {
    if (id != m_currentShaderId) {
      glUseProgram(id);
      m_currentShaderId = id;
    }
  }

  void ShaderManager::setBool(const std::string &name, bool value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void ShaderManager::setInt(const std::string &name, int value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1i(uniformLocation, value);
  }

  void ShaderManager::setUint(const std::string &name, unsigned int value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1ui(uniformLocation, value);
  }

  void ShaderManager::setFloat(const std::string &name, float value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform1f(uniformLocation, value);
  }

  void ShaderManager::setVector2(const std::string &name, const glm::vec2 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::setVector2(const std::string &name, float x, float y) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform2f(uniformLocation, x, y);
  }

  void ShaderManager::setVector3(const std::string &name, const glm::vec3 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::setVector3(const std::string &name, float x, float y, float z) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform3f(uniformLocation, x, y, z);
  }

  void ShaderManager::setVector4(const std::string &name, const glm::vec4 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
  }

  void ShaderManager::setVector4(const std::string &name, float x, float y, float z,
                                 float w) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniform4f(uniformLocation, x, y, z, w);
  }

  void ShaderManager::setMatrix2(const std::string &name, const glm::mat2 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void ShaderManager::setMatrix3(const std::string &name, const glm::mat3 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  void ShaderManager::setMatrix4(const std::string &name, const glm::mat4 &value) const {
    int uniformLocation = glGetUniformLocation(m_currentShaderId, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
  }

  // Compiles shader from code, returning its program ID
  ShaderId ShaderManager::compile(const char *shaderCode, unsigned int shaderType) {
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
      Logger::error("An error occurred while compiling shader:\n{}", errorLog);
    }

    return shaderId;
  }

  // Links the constituent shader programs to produce the complete shader program
  ShaderId ShaderManager::link(const std::vector<ShaderId> &shaderPrograms) {
    ShaderId program = glCreateProgram();
    for (ShaderId shader : shaderPrograms)
      glAttachShader(program, shader);
    glLinkProgram(program);

    // Validate shader linking
    int isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

    if (!isLinked) {
      int infoLogLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
      char *errorLog = new char[infoLogLength];
      glGetProgramInfoLog(program, infoLogLength, &infoLogLength, &errorLog[0]);
      Logger::error("An error occurred while linking shader:\n{}", errorLog);
    }

    return program;
  }
} // namespace TritiumEngine::Rendering