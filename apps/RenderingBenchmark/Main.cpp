#include "Scenes/CircleCollisionsScene.hpp"
#include "Scenes/CubeScene.hpp"
#include "Scenes/ParticlesBoxScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoader.hpp>
#include <TritiumEngine/Rendering/TextRendering/FontLoader.hpp>

using namespace RenderingBenchmark::Scenes;

static void setup(Application *app) {
  auto &window       = app->window;
  auto &sceneManager = app->sceneManager;

  // Setup resource paths
  ResourceManager<ShaderCode>::registerLoader<ShaderLoader>("Resources/Shaders/");
  ResourceManager<Font>::registerLoader<FontLoader>("Resources/Fonts/", "Hack-Regular");

  // Add window controls callbacks
  window.addKeyCallback(Key::ESCAPE, KeyState::START_PRESS, [app]() { app->stop(); });
  window.addKeyCallback(Key::R, KeyState::RELEASED,
                        [&sceneManager]() { sceneManager.reloadCurrentScene(); });
  window.addKeyCallback(Key::ENTER, KeyState::RELEASED,
                        [&sceneManager]() { sceneManager.nextScene(true); });
  window.setCloseCallback([app]() { app->stop(); });

  // Add scenes
  sceneManager.addScene<ParticlesBoxScene>();
  sceneManager.addScene<CubeScene>();
  sceneManager.addScene<CircleCollisionsScene>();
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