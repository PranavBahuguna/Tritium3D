#pragma once

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
    ShaderCode *Load(const std::string &filePath) override;
  };
} // namespace TritiumEngine::Rendering
