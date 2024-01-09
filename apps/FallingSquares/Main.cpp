#include "Application/FallingSquaresScene.hpp"

#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

void setup(Application *app) {
  // Setup resource paths
  ResourceManager<ShaderCode>::registerFactory(std::make_unique<ShaderLoaderFactory>(),
                                               "Resources/Shaders/");

  // Add window controls callbacks
  app->window.addKeyCallback(Key::ESCAPE, KeyState::PRESSED, [&app]() { app->stop(); });
  app->window.setCloseCallback([&app]() { app->stop(); });

  // Add some more key and mouse callbacks to test functionality
  app->window.addKeyCallback(Key::A, KeyState::PRESSED, []() { Logger::debug("A key pressed!"); });
  app->window.addKeyCallback(Key::A, KeyState::RELEASED,
                             []() { Logger::debug("A key released!"); });
  app->window.addKeyCallback(Key::R, KeyState::RELEASED,
                             [&app]() { app->sceneManager.reloadCurrentScene(); });
  app->window.addMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::PRESSED,
                                     []() { Logger::debug("Mouse button pressed!"); });
  app->window.addMouseButtonCallback(MouseButton::MOUSE_1, MouseButtonState::RELEASED,
                                     []() { Logger::debug("Mouse button released!"); });
  app->window.addMouseMoveCallback(
      [](double xPos, double yPos) { Logger::debug("Mouse at x:{}, y:{}", xPos, yPos); });
  app->window.addMouseScrollCallback([](double xOffSet, double yOffSet) {
    Logger::debug("Mouse scroll offset x:{}, y:{}", xOffSet, yOffSet);
  });

  // Setup scenes and systems
  auto mainScene = std::make_unique<FallingSquaresScene>();
  mainScene->addSystem<RenderSystem>();
  mainScene->addSystem<Gravity>(0.6f);
  mainScene->removeSystem<Gravity>();

  // Add scenes
  app->sceneManager.addScene(std::move(mainScene));
}

int main() {
#ifdef _DEBUG
  WindowSettings windowSettings{"main"};
#else
  Logger::Settings::levelMask = LogType::NODEBUG;
  WindowSettings windowSettings{"main", WindowHints::FULLSCREEN | WindowHints::AUTO_MINIMIZE};
#endif // _DEBUG

  try {
    auto *app = new Application("FallingSquares", windowSettings);
    setup(app);
    app->run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  Logger::info("Thank you for playing Falling Squares!");
  return EXIT_SUCCESS;
}