#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

namespace TritiumEngine::Rendering
{
  struct Material {
    glm::vec4 color;
    unsigned int shaderId;
  };
} // namespace TritiumEngine::Rendering

#endif // MATERIAL_HPP