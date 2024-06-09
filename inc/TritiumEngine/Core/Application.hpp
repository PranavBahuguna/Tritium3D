#pragma once

#include <TritiumEngine/Core/SceneManager.hpp>
#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include <chrono>

using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Core
{
  class Scene;

  using Clock     = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::steady_clock::time_point;

  class Application {
  public:
    Application(const std::string &name, const WindowSettings &windowSettings = WindowSettings{});
    Application(const Application &)           = delete;
    Application operator=(const Application &) = delete;
    virtual ~Application()                     = default;

    void run();
    void stop();
    bool isRunning() const;

    Window window;
    InputManager inputManager;
    ShaderManager shaderManager;
    SceneManager sceneManager;
    entt::registry registry;
    entt::dispatcher dispatcher;

    const std::string name;

  private:
    void initGLEW() const;

    bool m_isRunning = false;
    TimePoint m_currentTime;
    TimePoint m_prevFrameTime;
  };
} // namespace TritiumEngine::Core
