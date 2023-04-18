#pragma once

#include <TritiumEngine/Core/ResourceLoaderFactory.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class ShaderLoaderFactory : public ResourceLoaderFactory<Shader> {
  public:
    Shader *Load(const std::string &filePath) override;

  private:
    std::string ReadFile(const std::string &filePath) const;
  };
} // namespace TritiumEngine::Rendering
