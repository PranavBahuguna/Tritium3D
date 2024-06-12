#include "Scenes/CircleCollisionsScene.hpp"
#include "Components/Tags.hpp"
#include "Settings.hpp"

#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Systems/InstancedRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/Systems/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/Scripts/FpsStatsUI.hpp>

using Projection = Camera::Projection;

using namespace TritiumEngine::Utilities;
using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Settings;

namespace
{
  constexpr static glm::vec3 UI_CAMERA_POSITION = {0.f, 0.f, 1.f};
} // namespace

namespace RenderingBenchmark::Scenes
{
  CircleCollisionsScene::CircleCollisionsScene(const std::string &name, Application &app)
      : Scene(name, app), m_fpsDisplayCallback() {}

  void CircleCollisionsScene::init() {
    // Setup render settings
    RenderSettings textRenderSettings;
    textRenderSettings.enableBlend  = true;
    textRenderSettings.blendSFactor = GL_SRC_ALPHA;
    textRenderSettings.blendDFactor = GL_ONE_MINUS_SRC_ALPHA;

    // Setup systems
    addSystem<InstancedRenderSystem<MainCameraTag::value>>();
    addSystem<TextRenderSystem<MainCameraTag::value>>(textRenderSettings);

    // Setup camera
    auto &registry = m_app.registry;
    auto &window   = m_app.window;
    auto &input    = m_app.inputManager;

    auto camera  = registry.create();
    float aspect = window.getFrameAspect();
    registry.emplace<Camera>(camera, Projection::ORTHOGRAPHIC, VERTICAL_SCREEN_UNITS * aspect,
                             VERTICAL_SCREEN_UNITS, Transform{UI_CAMERA_POSITION});
    registry.emplace<MainCameraTag>(camera);

    // Setup UI
    auto fpsStatsUI = registry.create();
    auto &fpsStatsScript =
        registry.emplace<NativeScript>(fpsStatsUI, std::make_unique<FpsStatsUI>(m_app));
    fpsStatsScript.getInstance().setEnabled(false);

    // Setup controls
    m_fpsDisplayCallback = input.addKeyCallback(Key::F, KeyState::RELEASED, [&fpsStatsScript]() {
      fpsStatsScript.getInstance().toggleEnabled();
    });

    // Setup circles (TODO)
  }

  void CircleCollisionsScene::dispose() { m_app.inputManager.removeCallback(m_fpsDisplayCallback); }
} // namespace RenderingBenchmark::Scenes