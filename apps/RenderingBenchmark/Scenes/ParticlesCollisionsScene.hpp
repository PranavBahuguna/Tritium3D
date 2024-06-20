#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Utilities/CameraController.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Input;

namespace TritiumEngine::Core
{
  class Application;
}

namespace RenderingBenchmark::Scenes
{
  using Application = TritiumEngine::Core::Application;

  class ParticleCollisionsScene : public Scene {
  public:
    ParticleCollisionsScene(const std::string &name, Application &app);

  protected:
    void init() override;
    void dispose() override;

    CameraController m_cameraController;
    CallbackId m_fpsDisplayCallback;
  };
} // namespace RenderingBenchmark::Scenes