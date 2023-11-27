#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  Application::Application(const std::string &name, Window *window, std::unique_ptr<Scene> scene)
      : m_name(name), m_window(window), m_scene(std::move(scene)) {
    m_scene->registerWithApplication(*this);
    initGLEW();
  }

  /** @brief Starts running the application */
  void Application::run() {
    if (m_isRunning) {
      Logger::warn("[Application] App '{}' is already running!", m_name);
      return;
    }

    m_isRunning = true;
    Logger::info("[Application] App '{}' running...", m_name);
    m_scene->init();

    // Run main application loop
    while (m_isRunning) {
      // Update time
      m_currentTime   = Clock::now();
      float deltaTime = std::chrono::duration<float>(m_currentTime - m_prevFrameTime).count();
      m_prevFrameTime = m_currentTime;

      // Update window
      m_window->update();
      m_window->refresh();

      // Update scene
      m_scene->update(deltaTime);
    }

    Logger::info("[Application] App '{}' stopped.", m_name);
  }

  /** @brief Sets application flagged to stop */
  void Application::stop() { m_isRunning = false; }

  /** @brief Obtains the current scene's entity */
  entt::entity Application::getCurrentSceneEntity() const { return m_scene->getEntity(); }

  void Application::initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
      throw std::runtime_error("GLEW could not be initialised!");
  }
} // namespace TritiumEngine::Core