#include "Scenes/ParticlesBoxScene.hpp"
#include "Scenes/CubeScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoader.hpp>
#include <TritiumEngine/Rendering/TextRendering/FontLoader.hpp>

using namespace RenderingBenchmark::Scenes;

static void setup(Application *app) {
  // Setup resource paths
  ResourceManager<ShaderCode>::registerLoader<ShaderLoader>("Resources/Shaders/");
  ResourceManager<Font>::registerLoader<FontLoader>("Resources/Fonts/", "Hack-Regular");

  // Add window controls callbacks
  app->window.addKeyCallback(Key::ESCAPE, KeyState::PRESSED, [app]() { app->stop(); });
  app->window.addKeyCallback(Key::R, KeyState::RELEASED,
                             [app]() { app->sceneManager.reloadCurrentScene(); });
  app->window.addKeyCallback(Key::LEFT, KeyState::RELEASED,
                             [app]() { app->sceneManager.prevScene(true); });
  app->window.addKeyCallback(Key::RIGHT, KeyState::RELEASED,
                             [app]() { app->sceneManager.nextScene(true); });
  app->window.setCloseCallback([app]() { app->stop(); });

  // Add scenes
  app->sceneManager.addScene(std::move(std::make_unique<ParticlesBoxScene>()));
  app->sceneManager.addScene(std::move(std::make_unique<CubeScene>()));
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