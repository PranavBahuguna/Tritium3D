#ifndef SHADER_LOADER_FACTORY_HPP
#define SHADER_LOADER_FACTORY_HPP

#include <TritiumEngine/Core/ResourceLoaderFactory.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  // Struct wrapper for shader code data
  struct ShaderCode {
    std::string data;
  };

  class ShaderLoaderFactory : public ResourceLoaderFactory<ShaderCode> {
  public:
    ShaderCode *load(const std::string &filePath) override;
  };
} // namespace TritiumEngine::Rendering

#endif // SHADER_LOADER_FACTORY_HPP