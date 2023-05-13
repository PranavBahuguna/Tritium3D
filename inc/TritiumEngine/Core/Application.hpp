#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <TritiumEngine/Entities/World.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <chrono>
#include <memory>
#include <string>

using namespace TritiumEngine::Entities;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Core
{
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::steady_clock::time_point TimePoint;

  class Application {
  public:
    Application(const std::string &name, const Camera &camera, Window *window);

    World &GetWorld() const;
    ShaderManager &GetShaderManager() const;
    RenderSystem &GetRenderSystem() const;

    void Run();
    void Stop();

  private:
    void InitGLEW();

    std::string m_name;
    std::unique_ptr<World> m_world;
    std::unique_ptr<ShaderManager> m_shaderManager;
    std::unique_ptr<RenderSystem> m_renderSystem;
    Camera m_camera;
    Window *m_window;

    bool m_isRunning = false;

    TimePoint m_currentTime;
    TimePoint m_prevFrameTime = Clock::now();
  };
} // namespace TritiumEngine::Core

#endif // APPLICATION_HPP
