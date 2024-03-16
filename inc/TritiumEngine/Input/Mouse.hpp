#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace TritiumEngine::Input::Mouse
{
  static constexpr int NUM_MOUSE_BUTTONS = 8;

  enum class MouseButtonState { RELEASED = GLFW_RELEASE, PRESSED = GLFW_PRESS };

  enum class MouseButton {
    MOUSE_1 = GLFW_MOUSE_BUTTON_1, // Left mouse button
    MOUSE_2 = GLFW_MOUSE_BUTTON_2, // Right mouse button
    MOUSE_3 = GLFW_MOUSE_BUTTON_3, // Middle mouse button

    // Additional mouse buttons
    MOUSE_4 = GLFW_MOUSE_BUTTON_4,
    MOUSE_5 = GLFW_MOUSE_BUTTON_5,
    MOUSE_6 = GLFW_MOUSE_BUTTON_6,
    MOUSE_7 = GLFW_MOUSE_BUTTON_7,
    MOUSE_8 = GLFW_MOUSE_BUTTON_8,
  };
} // namespace TritiumEngine::Input::Mouse
