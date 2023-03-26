#pragma once

#include <GLFW/glfw3.h>

namespace Tritium3D::UI
{
  class WindowManager {
  public:
    static GLFWwindow *CreateWindow();

  private:
    GLFWwindow *_window;
  };
} // namespace Tritium3D::UI