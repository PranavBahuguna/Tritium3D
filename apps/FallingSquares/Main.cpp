#ifdef _DEBUG
#define WINDOW_SETTINGS WindowMode::WINDOWED
#else
#define WINDOW_SETTINGS WindowMode::FULLSCREEN
#endif // DEBUG

#define SCREEN_UNITS 100.0f
#define NUM_SQUARES  1000

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/RenderData.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Input::Keyboard;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

class FallingSquaresScene : public Scene {
public:
  void init() override{};
  void dispose() override{};
};

struct Rigidbody {
  glm::vec3 velocity;
};

class Gravity : public System {
public:
  Gravity(float g = 0.05f) : System(), m_g(g) {}

  void update(float dt) override {
    m_app->registry.view<Rigidbody, Transform>().each(
        [&](auto entity, Rigidbody &rigidbody, Transform &transform) {
          float acceleration = -m_g * dt;
          rigidbody.velocity += glm::vec3(0.0f, acceleration, 0.0f);
          transform.setPosition(transform.getPosition() + rigidbody.velocity);
        });
  }

private:
  float m_g;
};

void run() {
  // Setup window
#ifdef _DEBUG
  Window *window = new Window("main");
#else
  Logger::Settings::levelMask = LogType::NODEBUG;
  Window *window = new Window("main", WindowSettings::FULLSCREEN | WindowSettings::AUTO_MINIMIZE);
#endif // _DEBUG

  // Setup resource paths
  ResourceManager<ShaderCode>::registerFactory(std::make_unique<ShaderLoaderFactory>(),
                                               "Resources/Shaders/");

  // Setup scene and systems
  auto mainScene = std::make_unique<FallingSquaresScene>();
  mainScene->addSystem(std::make_unique<RenderSystem>());
  mainScene->addSystem(std::make_unique<Gravity>());

  // Setup engine
  Application app("FallingSquares", window, std::move(mainScene));
  ShaderManager *shaderManager = &app.shaderManager;
  entt::registry *registry     = &app.registry;
  entt::entity sceneEntity     = app.getCurrentSceneEntity();

  // Add window controls callbacks
  window->addKeyCallback(Key::ESCAPE, KeyState::PRESSED, [&app]() { app.stop(); });
  window->setCloseCallback([&app]() { app.stop(); });

  // Add some more key and mouse callbacks to test functionality
  window->addKeyCallback(Key::A, KeyState::PRESSED, []() { Logger::debug("A key pressed!"); });
  window->addKeyCallback(Key::A, KeyState::RELEASED, []() { Logger::debug("A key released!"); });
  window->addMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::PRESSED,
                                 []() { Logger::debug("Mouse button pressed!"); });
  window->addMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::RELEASED,
                                 []() { Logger::debug("Mouse button released!"); });
  window->addMouseMoveCallback(
      [](double xPos, double yPos) { Logger::debug("Mouse at x:{}, y:{}", xPos, yPos); });
  window->addMouseScrollCallback([](double xOffSet, double yOffSet) {
    Logger::debug("Mouse scroll offset x:{}, y:{}", xOffSet, yOffSet);
  });

  // Setup camera
  float screenWidth  = (float)window->getWidth();
  float screenHeight = (float)window->getHeight();
  float aspect       = screenWidth / screenHeight;

  registry->emplace<Camera>(sceneEntity, ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                            SCREEN_UNITS, 0.1f, 100.0f);

  // Create squares
  Transform transform;
  transform.setPosition(glm::vec3(-40.0f, 40.0f, 0.0f));
  transform.setScale(glm::vec3(5.0f, 5.0f, 1.0f));

  std::vector<float> vertices = {
      0.5f,  0.5f,  // top right
      0.5f,  -0.5f, // bottom right
      -0.5f, -0.5f, // bottom left
      -0.5f, 0.5f   // top left
  };

  std::vector<unsigned int> indices = {
      0, 1, 2, // first triangle
      0, 2, 3  // second triangle
  };

  RenderData renderData{2, vertices, indices};
  ShaderId defaultShader = app.shaderManager.get("default");

  const glm::vec4 color(1.0f, 0.5f, 0.2f, 1.0f);
  Material material(color, defaultShader);
  Renderable renderable(GL_TRIANGLES, renderData);
  Rigidbody rigidbody{glm::vec3()};

  for (int i = 0; i < 10; ++i) {
    glm::vec3 pos = transform.getPosition();
    transform.setPosition({pos.x + 10, pos.y, pos.z});

    entt::entity entity = registry->create();
    registry->emplace<Transform>(entity, transform);
    registry->emplace<Renderable>(entity, renderable);
    registry->emplace<Material>(entity, material);
    registry->emplace<Rigidbody>(entity, rigidbody);
  }

  app.run();
}

int main() {
  try {
    run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  Logger::info("Thank you for playing Falling Squares!");
  return EXIT_SUCCESS;
}