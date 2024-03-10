#pragma once

#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Rendering/Color.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace RenderingBenchmark::Systems
{
  class BoxContainerSystem : public System {
  public:
    BoxContainerSystem(float boxSize = 100.f);
    void update(float dt) override;

  private:
    void updateColor(glm::vec3 &velocity, Color &color);

    float m_boxSize;
  };
} // namespace RenderingBenchmark::Systems