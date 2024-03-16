#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace TritiumEngine::Input::Cursor
{
  enum class CursorState {
    NORMAL   = GLFW_CURSOR_NORMAL,   // Show cursor
    HIDDEN   = GLFW_CURSOR_HIDDEN,   // Hide cursor if hovering over the window
    DISABLED = GLFW_CURSOR_DISABLED, // Disables and hides cursor entirely
    CAPTURED = GLFW_CURSOR_CAPTURED, // Show cursor, but restrict to the window area only
  };
} // namespace TritiumEngine::Input::Cursor
