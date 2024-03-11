#pragma once

#include <TritiumEngine/Core/Scene.hpp>

using namespace TritiumEngine::Core;

namespace RenderingBenchmark::Scenes
{
  class TestScene : public Scene {
  public:
    TestScene();

  protected:
    void init() override;
    void dispose() override;
  };
} // namespace RenderingBenchmark::Scenes