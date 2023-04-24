#pragma once

#include <TritiumEngine/Rendering/ShaderManager.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace TritiumEngine::Rendering
{
  struct Material {
    glm::vec4 color;
    ShaderId shaderId;
  };
} // namespace TritiumEngine::Rendering
