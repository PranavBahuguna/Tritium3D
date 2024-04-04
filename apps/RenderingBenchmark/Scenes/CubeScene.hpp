#pragma once

#include <TritiumEngine/Core/Scene.hpp>
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
    CubeScene();

  protected:
    void init() override;
    void onRegister() override;
    void dispose() override;

  private:
    void setupSystems();
    void setupUI();
    void setupCameras();
    void setupCameraController();
    void setupControls();
    void generateCubes(int nCubes);

    CameraController m_cameraController;
    entt::entity m_fpsStatsUI    = entt::null;
    entt::entity m_cameraStatsUI = entt::null;
    CallbackId m_callbacks[2];
  };
} // namespace RenderingBenchmark::Scenes