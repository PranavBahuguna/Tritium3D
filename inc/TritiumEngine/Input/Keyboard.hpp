#pragma once

#include <GLFW/glfw3.h>

namespace TritiumEngine::Input
{
  static constexpr int NUM_KEYS = 512;

  class Keyboard {
  public:
    static void KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
    static bool GetKeyToggled(int key);

  private:
    struct KeyState {
    public:
      void Update(int action);
      bool IsToggled();

    private:
      int m_oldState;
      int m_newState;
    };

    Keyboard() {} // prevent construction of this class

    static bool ValidateKey(int key);
    static KeyState _keys[NUM_KEYS];
  };
} // namespace TritiumEngine::Input