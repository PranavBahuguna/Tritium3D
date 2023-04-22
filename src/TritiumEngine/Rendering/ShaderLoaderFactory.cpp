#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <fstream>
#include <sstream>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  Shader *ShaderLoaderFactory::Load(const std::string &filePath) {
    // Load vertex and fragment shader code from files
    std::string vertexShaderCode = ReadFile(filePath + ".vert");
    std::string fragmentShaderCode = ReadFile(filePath + ".frag");

    return new Shader(vertexShaderCode, fragmentShaderCode);
  }

  // Extracts file contents to string
  std::string ShaderLoaderFactory::ReadFile(const std::string &filePath) const {
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
      Logger::Log(ERROR, "Could not read from file {}", filePath);
    }

    return fileData;
  }
} // namespace TritiumEngine::Rendering