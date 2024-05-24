#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/ColorGradient.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/CameraController.hpp>

#include <entt/entt.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

namespace RenderingBenchmark::Scenes
{
  class CubeScene : public Scene {
  public:
    CubeScene(const std::string &name, Application &app);

  protected:
    void init() override;
    void dispose() override;

  private:
    void generateParticles(int nParticles);

    CameraController m_cameraController;
    CallbackId m_callbacks[2];
    ColorGradient m_gradient;
  };
} // namespace RenderingBenchmark::Scenes