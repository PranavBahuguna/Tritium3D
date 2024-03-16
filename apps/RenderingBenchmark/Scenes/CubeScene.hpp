#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace RenderingBenchmark::Scenes
{
  class CubeScene : public Scene {
  public:
    CubeScene();

  protected:
    void init() override;
    void dispose() override;

  private:
    void setupControls();

    std::array<CallbackId, 1> m_callbackIds;
  };
} // namespace RenderingBenchmark::Scenes