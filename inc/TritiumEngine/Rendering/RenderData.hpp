#pragma once

#include <vector>

namespace TritiumEngine::Rendering
{
  struct RenderData {
    unsigned int vertexStride = 0;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
  };
} // namespace TritiumEngine::Rendering