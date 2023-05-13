#ifdef _DEBUG
#define WINDOW_SETTINGS WindowMode::WINDOWED
#else
#define WINDOW_SETTINGS WindowMode::FULLSCREEN
#endif // DEBUG

#define SCREEN_UNITS 100.0f
#define NUM_SQUARES  1000

#include "Square.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Input::Keyboard;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

int main() {
  try {
    // Setup Logger
#ifdef _DEBUG
    Window *window = new Window("main");
#else
    Logger::Settings::levelMask = LogType::NODEBUG;
    Window *window = new Window("main", WindowSettings::FULLSCREEN | WindowSettings::AUTO_MINIMIZE);
#endif // _DEBUG

    // Setup camera
    float screenWidth  = (float)window->GetWidth();
    float screenHeight = (float)window->GetHeight();
    float aspect       = screenWidth / screenHeight;

    Camera camera(ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f);

    Application app("FallingSquares", camera, window);
    World &world                 = app.GetWorld();
    ShaderManager &shaderManager = app.GetShaderManager();
    RenderSystem &renderSystem   = app.GetRenderSystem();

    // Add window controls callbacks
    window->AddKeyCallback(Key::ESCAPE, KeyState::PRESSED, [&app]() { app.Stop(); });
    window->SetCloseCallback([&app]() { app.Stop(); });

    // Add some more key and mouse callbacks to test functionality
    window->AddKeyCallback(Key::A, KeyState::PRESSED, []() { Logger::Debug("A key pressed!"); });
    window->AddKeyCallback(Key::A, KeyState::RELEASED, []() { Logger::Debug("A key released!"); });
    window->AddMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::PRESSED,
                                   []() { Logger::Debug("Mouse button pressed!"); });
    window->AddMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::RELEASED,
                                   []() { Logger::Debug("Mouse button released!"); });
    window->AddMouseMoveCallback(
        [](double xPos, double yPos) { Logger::Debug("Mouse at x:{}, y:{}", xPos, yPos); });
    window->AddMouseScrollCallback([](double xOffSet, double yOffSet) {
      Logger::Debug("Mouse scroll offset x:{}, y:{}", xOffSet, yOffSet);
    });

    // Setup resource paths
    ResourceManager<ShaderCode>::Register(
        std::unique_ptr<ShaderLoaderFactory>(new ShaderLoaderFactory()), "Resources/Shaders/");

    // Create squares
    Transform transform;
    transform.SetPosition(glm::vec3(-40.0f, 40.0f, 0.0f));
    transform.SetScale(glm::vec3(5.0f, 5.0f, 1.0f));

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
    ShaderId defaultShader = shaderManager.Get("default");

    const glm::vec4 color(1.0f, 0.5f, 0.2f, 1.0f);
    const auto &material   = std::make_shared<Material>(color, defaultShader);
    const auto &renderable = std::make_shared<Renderable>(GL_TRIANGLES, renderData);

    for (int i = 0; i < 10; ++i) {
      world.AddEntity(new Square(transform, renderable, material));
      glm::vec3 pos = transform.GetPosition();
      transform.SetPosition({pos.x + 10, pos.y, pos.z});
    }

    app.Run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  Logger::Info("Thank you for playing Falling Squares!");
  return EXIT_SUCCESS;
}