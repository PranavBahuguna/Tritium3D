#pragma once

#include <TritiumEngine/Core/ResourceLoader.hpp>
#include <TritiumEngine/Rendering/ShaderCode.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class ShaderLoader : public ResourceLoader<ShaderCode> {
  public:
    ShaderCode *load(const std::string &filePath) override;
  };
} // namespace TritiumEngine::Rendering