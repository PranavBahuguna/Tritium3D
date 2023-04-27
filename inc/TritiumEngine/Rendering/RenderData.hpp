#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP

#include <vector>

namespace TritiumEngine::Rendering
{
  struct RenderData {
    unsigned int vertexStride = 0;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
  };
} // namespace TritiumEngine::Rendering

#endif // RENDER_DATA_HPP