#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/CameraController.hpp>

#include <entt/entity/entity.hpp>

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
    void setupCameras();
    void setupControls();

    entt::entity m_sceneCamera{};
    CameraController m_cameraController;
  };
} // namespace RenderingBenchmark::Scenes