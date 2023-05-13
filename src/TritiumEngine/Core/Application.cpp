#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  Application::Application(const std::string &name, const Camera &camera, Window *window)
      : m_name(name), m_world(std::unique_ptr<World>(new World())),
        m_shaderManager(std::unique_ptr<ShaderManager>(new ShaderManager())),
        m_renderSystem(std::unique_ptr<RenderSystem>(new RenderSystem(m_shaderManager.get()))),
        m_camera(camera), m_window(window) {
    InitGLEW();
  }

  World &Application::GetWorld() const { return *m_world.get(); }

  ShaderManager &Application::GetShaderManager() const { return *m_shaderManager; }

  RenderSystem &Application::GetRenderSystem() const { return *m_renderSystem; }

  /** Starts running the application. */
  void Application::Run() {
    if (m_isRunning)
      return;

    Logger::Info("[Application] App '{}' running...", m_name);
    m_isRunning = true;
    m_world->Init();

    // Run main application loop
    while (m_isRunning) {
      // Update time
      m_currentTime   = Clock::now();
      float deltaTime = std::chrono::duration<float>(m_currentTime - m_prevFrameTime).count();
      m_prevFrameTime = m_currentTime;

      // Update window
      m_window->Update();
      m_window->Refresh();

      // Update world and render
      m_world->Update(deltaTime);
      m_renderSystem->Draw(m_camera, *m_world);
    }

    Logger::Info("[Application] App '{}' stopped.", m_name);
  }

  /** Sets application flagged to stop. */
  void Application::Stop() { m_isRunning = false; }

  void Application::InitGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
      throw std::runtime_error("GLEW could not be initialised!");
  }
} // namespace TritiumEngine::Core