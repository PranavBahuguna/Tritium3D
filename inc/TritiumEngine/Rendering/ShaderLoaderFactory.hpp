#pragma once

#include <TritiumEngine/Core/ResourceLoaderFactory.hpp>
#include <TritiumEngine/Rendering/ShaderCode.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class ShaderLoaderFactory : public ResourceLoaderFactory<ShaderCode> {
  public:
    ShaderCode *load(const std::string &filePath) override;
  };
} // namespace TritiumEngine::Rendering