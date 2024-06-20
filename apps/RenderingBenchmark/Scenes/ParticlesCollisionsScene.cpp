#include "Scenes/ParticlesCollisionsScene.hpp"
#include "Components/Tags.hpp"
#include "Settings.hpp"
#include "Systems/CirclesRenderSystem.hpp"

#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Core/Components/Rigidbody.hpp>
#include <TritiumEngine/Physics/Components/AABB.hpp>
#include <TritiumEngine/Rendering/ColorGradient.hpp>
#include <TritiumEngine/Rendering/Components/Camera.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/Systems/InstancedRenderSystem.hpp>
#include <TritiumEngine/Rendering/Systems/StandardRenderSystem.hpp>
#include <TritiumEngine/Rendering/TextRendering/Systems/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/Random/GridDistribution.hpp>
#include <TritiumEngine/Utilities/Random/Position.hpp>
#include <TritiumEngine/Utilities/Scripts/FpsStatsUI.hpp>

using Projection = Camera::Projection;

using namespace TritiumEngine::Utilities;
using namespace TritiumEngine::Physics;
using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Settings;
using namespace RenderingBenchmark::Systems;

namespace
{
  constexpr static glm::vec3 UI_CAMERA_POSITION = {0.f, 0.f, 1.f};
  constexpr static glm::quat PARTICLE_ROTATION  = {1.f, 0.f, 0.f, 0.f};
  constexpr static float PARTICLE_SCALE         = 6.f;
  constexpr static float PARTICLE_VELOCITY      = 20.f;
  constexpr static float GRID_SIZE_X            = 30000.f;
  constexpr static float GRID_SIZE_Y            = 30000.f;
  constexpr static size_t NUM_GRID_COLS         = 1000;
  constexpr static size_t NUM_GRID_ROWS         = 1000;

  constexpr static int NUM_PARTICLES        = static_cast<int>(NUM_GRID_COLS * NUM_GRID_COLS);
  constexpr static float GRID_CELL_WIDTH    = GRID_SIZE_X / static_cast<float>(NUM_GRID_COLS);
  constexpr static float GRID_CELL_HEIGHT   = GRID_SIZE_Y / static_cast<float>(NUM_GRID_ROWS);
  constexpr static float GRID_MIN_DIMENSION = std::min(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
  constexpr static float DISPLACEMENT =
      ((GRID_MIN_DIMENSION - 2 * PARTICLE_SCALE) / GRID_MIN_DIMENSION) - 0.005f;
} // namespace

namespace RenderingBenchmark::Scenes
{
  ParticleCollisionsScene::ParticleCollisionsScene(const std::string &name, Application &app)
      : Scene(name, app), m_fpsDisplayCallback(), m_cameraController(app.inputManager) {
    // Setup camera controller
    m_cameraController.mapKey(Key::LEFT, CameraAction::MOVE_LEFT);
    m_cameraController.mapKey(Key::RIGHT, CameraAction::MOVE_RIGHT);
    m_cameraController.mapKey(Key::DOWN, CameraAction::MOVE_DOWN);
    m_cameraController.mapKey(Key::UP, CameraAction::MOVE_UP);
    m_cameraController.mapKey(Key::Z, CameraAction::ZOOM_IN);
    m_cameraController.mapKey(Key::X, CameraAction::ZOOM_OUT);
    m_cameraController.moveSpeed           = 100.f;
    m_cameraController.minOrthographicZoom = 0.01f;
    m_cameraController.maxOrthographicZoom = 50.f;
  }

  void ParticleCollisionsScene::init() {
    // Setup render settings
    RenderSettings textRenderSettings;
    textRenderSettings.enableBlend  = true;
    textRenderSettings.blendSFactor = GL_SRC_ALPHA;
    textRenderSettings.blendDFactor = GL_ONE_MINUS_SRC_ALPHA;

    // Setup systems
    addSystem<StandardRenderSystem<MainCameraTag::value>>();
    addSystem<CirclesRenderSystem<MainCameraTag::value>>(RenderSettings{}, 8000, 80000);
    addSystem<TextRenderSystem<UiCameraTag::value>>(textRenderSettings);

    auto &registry      = m_app.registry;
    auto &window        = m_app.window;
    auto &input         = m_app.inputManager;
    auto &shaderManager = m_app.shaderManager;

    // Setup cameras
    float aspect      = window.getFrameAspect();
    float camWidth    = VERTICAL_SCREEN_UNITS * aspect;
    float camHeight   = VERTICAL_SCREEN_UNITS;
    const auto camPos = glm::vec3{0.f, 0.f, 1.f};

    auto mainCamera = registry.create();
    auto &mainCameraComponent =
        registry.emplace<Camera>(mainCamera, Projection::ORTHOGRAPHIC, camWidth, camHeight, camPos);
    registry.emplace<MainCameraTag>(mainCamera);
    m_cameraController.init(mainCameraComponent, false, false);

    auto uiCamera = registry.create();
    registry.emplace<Camera>(uiCamera, Projection::ORTHOGRAPHIC, camWidth, camHeight, camPos);
    registry.emplace<UiCameraTag>(uiCamera);

    // Setup UI
    auto fpsStatsUI = registry.create();
    auto &fpsStatsScript =
        registry.emplace<NativeScript>(fpsStatsUI, std::make_unique<FpsStatsUI>(m_app));
    fpsStatsScript.getInstance().setEnabled(false);

    // Setup controls
    m_fpsDisplayCallback = input.addKeyCallback(Key::F, KeyState::RELEASED, [&fpsStatsScript]() {
      fpsStatsScript.getInstance().toggleEnabled();
    });

    // Create background quad particle container
    auto backgroundQuad = registry.create();
    registry.emplace<Renderable>(backgroundQuad, GL_TRIANGLES,
                                 Primitives::createQuad(GRID_SIZE_X, GRID_SIZE_Y));
    registry.emplace<Transform>(backgroundQuad, Transform{});
    registry.emplace<Shader>(backgroundQuad, shaderManager.get("default"));
    registry.emplace<Color>(backgroundQuad, COLOR_BLACK);

    // Setup color gradient for the particles
    ColorGradient gradient;
    gradient.addColorPoint(COLOR_RED, 0.f);
    gradient.addColorPoint(COLOR_MAGENTA, 1.f);

    // Create instanced renderable template for particles
    auto particleTemplate = registry.create();
    auto &renderable      = registry.emplace<InstancedRenderable>(
        particleTemplate, GL_POINTS, Primitives::createPoint2d(), NUM_PARTICLES);

    // Create particles in random grid distribtion pattern
    Random::GridDistribution dist{NUM_GRID_COLS, NUM_GRID_COLS, GRID_CELL_WIDTH, GRID_CELL_HEIGHT,
                                  DISPLACEMENT};
    for (int i = 0; i < NUM_PARTICLES; ++i) {
      auto particle = registry.create();
      registry.emplace<InstanceTag>(particle, renderable.getInstanceId());
      registry.emplace<Transform>(particle, dist.getNext(), PARTICLE_ROTATION,
                                  PARTICLE_SCALE * glm::vec3(1.f));
      registry.emplace<Color>(particle, gradient.getColor((float)i / NUM_PARTICLES));
      registry.emplace<Rigidbody>(particle, Random::Velocity2D(PARTICLE_VELOCITY));
      registry.emplace<AABB>(particle, PARTICLE_SCALE, PARTICLE_SCALE);
    }
  }

  void ParticleCollisionsScene::dispose() {
    m_app.inputManager.removeCallback(m_fpsDisplayCallback);
    m_cameraController.dispose();
  }
} // namespace RenderingBenchmark::Scenes