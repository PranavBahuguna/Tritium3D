#pragma once

#include <TritiumEngine/Rendering/Shader.hpp>

#include <glm/glm.hpp>

#include <memory>

namespace TritiumEngine::Rendering
{
  struct Material {
    glm::vec4 color;
    std::shared_ptr<Shader> shader;
  };
} // namespace TritiumEngine::Rendering
