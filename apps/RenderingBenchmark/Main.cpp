#include "Scenes/ParticlesBoxScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

using namespace RenderingBenchmark::Scenes;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

static void setup(Application *app) {
  // Setup resource paths
  ResourceManager<ShaderCode>::registerFactory(std::make_unique<ShaderLoaderFactory>(),
                                               "Resources/Shaders/");

  // Add window controls callbacks
  app->window.addKeyCallback(Key::ESCAPE, KeyState::PRESSED, [app]() { app->stop(); });
  app->window.setCloseCallback([app]() { app->stop(); });
  app->window.addKeyCallback(Key::R, KeyState::RELEASED,
                             [app]() { app->sceneManager.reloadCurrentScene(); });

  // Add scenes
  app->sceneManager.addScene(std::move(std::make_unique<ParticlesBoxScene>()));

  // Additional OpenGL settings
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
}

int main() {
#ifdef _DEBUG
  WindowSettings windowSettings{"main"};
#else
  Logger::Settings::levelMask = LogType::NODEBUG;
  WindowSettings windowSettings{"main", WindowHints::FULLSCREEN | WindowHints::AUTO_MINIMIZE};
#endif // _DEBUG

  try {
    auto *app = new Application("RenderingBenchmark", windowSettings);
    setup(app);
    app->run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  Logger::info("[RenderingBenchmark] Program exited successfully!");
  return EXIT_SUCCESS;
}