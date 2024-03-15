#pragma once

#include <vector>

namespace TritiumEngine::Rendering
{
  struct RenderData {
    int vertexStride = 0;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
  };
} // namespace TritiumEngine::Rendering