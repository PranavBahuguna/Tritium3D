#pragma once

#include <TritiumEngine/Core/System.hpp>

#include <entt/entt.hpp>

using namespace TritiumEngine::Core;

namespace RenderingBenchmark::Systems
{
  class FpsDisplaySystem : public System {
  public:
    FpsDisplaySystem();
    void init() override;
    void dispose() override;
    void update(float dt) override;

  private:
    entt::entity m_fpsText{};
    entt::entity m_frameTimeText{};
  };
} // namespace RenderingBenchmark::Scenes