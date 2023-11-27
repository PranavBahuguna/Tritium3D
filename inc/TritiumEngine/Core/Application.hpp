#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <TritiumEngine/Rendering/ShaderManager.hpp>

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include <chrono>
#include <memory>

using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Rendering
{
  class Window;
}

namespace TritiumEngine::Core
{
  class Scene;

  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::steady_clock::time_point TimePoint;

  class Application {
  public:
    Application(const std::string &name, Window *window, std::unique_ptr<Scene> scene);

    void run();
    void stop();

    entt::entity getCurrentSceneEntity() const;

    entt::registry registry;
    entt::dispatcher dispatcher;
    ShaderManager shaderManager;

  private:
    void initGLEW();

    std::string m_name;
    Window *m_window;
    std::unique_ptr<Scene> m_scene;

    bool m_isRunning = false;

    TimePoint m_currentTime;
    TimePoint m_prevFrameTime = Clock::now();
  };
}

#endif // APPLICATION_HPP
