#include <Tritium3D/Rendering/Shader.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace Tritium3D::Rendering
{
  Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    // Load vertex and fragment shader program data from files
    std::string vertexData = ReadFile(vertexPath);
    std::string fragmentData = ReadFile(fragmentPath);

    // Compile vertex and fragment shaders
    ShaderId vertexId = Compile(vertexData.c_str(), GL_VERTEX_SHADER);
    ShaderId fragmentId = Compile(fragmentData.c_str(), GL_FRAGMENT_SHADER);

    // Link the shaders
    m_id = Link(vertexId, fragmentId);
  }

  Shader::~Shader() { glDeleteProgram(m_id); }

  // Activate this shader program
  void Shader::Use() const { glUseProgram(m_id); }

  // Sets matrix4 in shader program
  void Shader::SetMatrix4(const std::string &name, const glm::mat4 &matrix4) const {
    int count;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);
    int uniformLocation = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix4));
  }

  // Obtains ID of this shader program
  ShaderId Shader::GetID() const { return m_id; }

  // Extracts file contents to string
  std::string Shader::ReadFile(const std::string &filePath) const {
    std::string fileData;
    std::ifstream fileStream;
    // Allow ifstream to throw exceptions
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      std::stringstream ss;
      fileStream.open(filePath);
      ss << fileStream.rdbuf();
      fileStream.close();
      fileData = ss.str();
    } catch (std::ifstream::failure e) {
      printf("Error, could not read from %s!\n", filePath.c_str());
    }

    return fileData;
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
} // namespace Tritium3D::Rendering