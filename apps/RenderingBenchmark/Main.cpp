#include "Scenes/CubeScene.hpp"
#include "Scenes/ParticlesBoxScene.hpp"
#include "Scenes/ParticlesCollisionsScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoader.hpp>
#include <TritiumEngine/Rendering/TextRendering/FontLoader.hpp>

using namespace RenderingBenchmark::Scenes;

static void setup(Application *app) {
  auto &input        = app->inputManager;
  auto &sceneManager = app->sceneManager;

  // Setup resource paths
  ResourceManager<ShaderCode>::registerLoader<ShaderLoader>("Resources/Shaders/");
  ResourceManager<Font>::registerLoader<FontLoader>("Resources/Fonts/", "Hack-Regular");

  // Add window controls callbacks
  input.addKeyCallback(Key::ESCAPE, KeyState::START_PRESS, [app]() { app->stop(); });
  input.addKeyCallback(Key::R, KeyState::RELEASED,
                       [&sceneManager]() { sceneManager.reloadCurrentScene(); });
  input.addKeyCallback(Key::ENTER, KeyState::RELEASED,
                       [&sceneManager]() { sceneManager.nextScene(true); });
  input.setCloseCallback([app]() { app->stop(); });

  // Add scenes
  sceneManager.addScene<ParticlesBoxScene>();
  sceneManager.addScene<CubeScene>();
  sceneManager.addScene<ParticleCollisionsScene>();
}

int main() {
  WindowSettings windowSettings{"main"};
#ifndef _DEBUG
  Logger::Settings::levelMask = LogType::NODEBUG;
  windowSettings.hints        = WindowHints::FULLSCREEN | WindowHints::AUTO_MINIMIZE;
  windowSettings.fullscreen   = true;
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