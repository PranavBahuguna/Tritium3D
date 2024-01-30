#pragma once

#include <TritiumEngine/Input/Cursor.hpp>
#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Input/Mouse.hpp>
#include <TritiumEngine/Utilities/EnumUtils.hpp>

#include <glm/glm.hpp>

#include <array>
#include <functional>
#include <string>

using namespace TritiumEngine::Input::Cursor;
using namespace TritiumEngine::Input::Keyboard;
using namespace TritiumEngine::Input::Mouse;

struct GLFWwindow;

namespace TritiumEngine::Rendering
{
  using KeyCallback         = std::function<void()>;
  using MouseButtonCallback = std::function<void()>;
  using MouseMoveCallback   = std::function<void(double, double)>; // xPos, yPos
  using MouseScrollCallback = std::function<void(double, double)>; // xOffset, yOffset
  using CloseCallback       = std::function<void()>;

  using KeyCallbackList         = std::vector<std::tuple<KeyState, KeyCallback>>;
  using MouseButtonCallbackList = std::vector<std::tuple<MouseButtonState, MouseButtonCallback>>;
  using MouseMoveCallbackList   = std::vector<MouseMoveCallback>;
  using MouseScrollCallbackList = std::vector<MouseScrollCallback>;

  enum class WindowHints : uint32_t {
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
  ENABLE_ENUM_FLAGS(WindowHints)

  struct WindowSettings {
    std::string name;
    WindowHints hints = WindowHints::DEFAULT;
    int width         = 800;
    int height        = 600;
  };

  class Window {
  public:
    Window(WindowSettings settings);
    Window(const Window &window) = delete;
    Window(Window &&) noexcept   = default;
    ~Window();

    void update() const;
    void refresh() const;
    void setCursorState(CursorState state) const;
    int getWidth() const;
    int getHeight() const;

    void addKeyCallback(Key key, KeyState state, KeyCallback callback);
    void addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                MouseButtonCallback callback);
    void addMouseMoveCallback(MouseMoveCallback callback);
    void addMouseScrollCallback(MouseScrollCallback callback);
    void setCloseCallback(CloseCallback callback);

  private:
    static Window *getUserPointer(GLFWwindow *windowHandle);

    static inline int s_nWindows = 0;

    GLFWwindow *m_windowHandle = nullptr;
    std::string m_name;
    int m_width;
    int m_height;

    std::array<KeyCallbackList, NUM_KEYS> m_keyCallbacks;
    std::array<MouseButtonCallbackList, NUM_MOUSE_BUTTONS> m_mouseButtonCallbacks;
    MouseMoveCallbackList m_mouseMoveCallbacks;
    MouseScrollCallbackList m_mouseScrollCallbacks;
    CloseCallback m_closeCallback;
  };
} // namespace TritiumEngine::Rendering