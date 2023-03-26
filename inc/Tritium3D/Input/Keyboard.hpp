#pragma once

#include <Tritium3D/Input/KeyState.hpp>

#include <GLFW/glfw3.h>

namespace Tritium3D::Input
{
  static constexpr int NUM_KEYS = 512;

  class Keyboard {
  public:
    static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
    static bool GetKeyToggled(int key);

  private:
    Keyboard() {} // prevent construction of this class

    static bool ValidateKey(int key);
    static KeyState _keys[NUM_KEYS];
  };
} // namespace Tritium3D::Input