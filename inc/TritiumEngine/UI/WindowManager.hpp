#pragma once

#include <GLFW/glfw3.h>

namespace TritiumEngine::UI
{
  class WindowManager {
  public:
    static GLFWwindow *CreateWindow();

  private:
    GLFWwindow *_window;
  };
} // namespace TritiumEngine::UI