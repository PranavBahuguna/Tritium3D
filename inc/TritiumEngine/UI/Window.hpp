#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace TritiumEngine::UI
{
  enum class WindowMode { WINDOWED, FULLSCREEN_WINDOWED, FULLSCREEN };

  class Window {
  public:
    Window(const std::string &name, WindowMode wMode, int width = 800, int height = 600);
    ~Window();

    bool IsInitialised() const;
    void Update() const;
    int GetWidth() const;
    int GetHeight() const;
    bool GetShouldClose() const;
    void Close() const;

  private:
    GLFWwindow *m_window;
    std::string m_name;
    WindowMode m_wMode;
    int m_width;
    int m_height;
  };
} // namespace TritiumEngine::UI