#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Utilities/EnumUtils.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>
#include <functional>
#include <string>

using namespace TritiumEngine::Input::Keyboard;

namespace TritiumEngine::Rendering
{
  enum class WindowSettings : uint32_t {
    FOCUSED        = 1u << 0, // Window automatically takes focus
    RESIZABLE      = 1u << 1, // Allows window to be manually resized by user input
    VISIBLE        = 1u << 2, // Makes the window visible
    DECORATED      = 1u << 3, // Adds window decorations (borders, buttons etc.)
    AUTO_MINIMIZE  = 1u << 4, // Automatically minimise window when not in focus
    ALWAYS_ON_TOP  = 1u << 5, // Automatically places window on top of all other screen UI
    MAXIMIZED      = 1u << 6, // Maximises window to full screen space
    CENTER_CURSOR  = 1u << 7, // Centers the cursor
    TRANSPARENT_FB = 1u << 8, // Blends screen elements behind the window with the framebuffer alpha
    AUTOFOCUS      = 1u << 9, // Focuses window every time it is opened

    NONE                = 0,                                         // Invisible window
    DEFAULT             = FOCUSED | RESIZABLE | VISIBLE | DECORATED, // Default settings
    FULLSCREEN          = FOCUSED | RESIZABLE | VISIBLE | MAXIMIZED, // Borderless fullscreen
    FULLSCREEN_WINDOWED = DEFAULT | MAXIMIZED                        // Windowed fullscreen
  };
  ENABLE_ENUM_FLAGS(WindowSettings)

  enum class CursorState : GLuint {
    NORMAL   = GLFW_CURSOR_NORMAL,   // Show cursor
    HIDDEN   = GLFW_CURSOR_HIDDEN,   // Hide cursor if hovering over the window
    DISABLED = GLFW_CURSOR_DISABLED, // Disables and hides cursor entirely
    CAPTURED = GLFW_CURSOR_CAPTURED, // Restrict cursor to the window area only
  };

  using KeyCallbacks = std::vector<std::tuple<KeyState, std::function<void()>>>;

  class Window {
  public:
    Window(const std::string &name, WindowSettings settings = WindowSettings::DEFAULT,
           unsigned int width = 800, unsigned int height = 600);
    Window(const Window &window) = delete;
    Window(Window &&) noexcept   = default;
    ~Window();

    void Update() const;
    void SetCursorState(CursorState state) const;
    void SetShouldClose() const;

    void AddKeyCallback(Key key, KeyState keyState, std::function<void()> action);

    int GetWidth() const;
    int GetHeight() const;
    int GetShouldClose() const;

  private:
    static inline int s_nWindows = 0;

    static Window *GetUserPointer(GLFWwindow *windowHandle);

    GLFWwindow *m_windowHandle = nullptr;
    std::string m_name;
    int m_width;
    int m_height;

    std::array<KeyCallbacks, NUM_KEYS> m_keyCallbacks;
  };
} // namespace TritiumEngine::Rendering

#endif // WINDOW_HPP