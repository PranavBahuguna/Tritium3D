#pragma once

#include <TritiumEngine/Input/Cursor.hpp>
#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Input/Mouse.hpp>
#include <TritiumEngine/Utilities/EnumUtils.hpp>

#include <glm/glm.hpp>

#include <array>
#include <functional>
#include <span>
#include <string>

using namespace TritiumEngine::Input::Cursor;
using namespace TritiumEngine::Input::Keyboard;
using namespace TritiumEngine::Input::Mouse;

struct GLFWwindow;

namespace TritiumEngine::Rendering
{
  // clang-format off
  using CallbackId              = uint32_t;
  using KeyCallback             = std::function<void(float)>;                 // deltaTime
  using KeyCallbackNoDt         = std::function<void()>;
  using MouseButtonCallback     = std::function<void(float)>;                 // deltaTime
  using MouseButtonCallbackNoDt = std::function<void()>;
  using MouseMoveCallback       = std::function<void(float, double, double)>; // deltaTime, xPos, yPos
  using MouseMoveCallbackNoDt   = std::function<void(double, double)>;        // xPos, yPos
  using MouseScrollCallback     = std::function<void(float, double, double)>; // deltaTime, xOffset, yOffset
  using MouseScrollCallbackNoDt = std::function<void(double, double)>;        // xOffset, yOffset
  using CloseCallback           = std::function<void()>;
  // clang-format on

  struct KeyCallbackItem {
    KeyState state;
    KeyCallback callback;
    CallbackId id;
  };

  struct MouseButtonCallbackItem {
    MouseButtonState state;
    MouseButtonCallback callback;
    CallbackId id;
  };

  struct MouseMoveCallbackItem {
    MouseMoveCallback callback;
    double prevXPos;
    double prevYPos;
    bool mouseFirstMove; 
    CallbackId id;
  };

  struct MouseScrollCallbackItem {
    MouseScrollCallback callback;
    CallbackId id;
  };

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
    int width         = 1280;
    int height        = 720;
  };

  class Window {
  public:
    Window(WindowSettings settings);
    Window(const Window &window) = delete;
    Window(Window &&) noexcept   = default;
    ~Window();

    void update(float dt);

    CallbackId addKeyCallback(Key key, KeyState state, KeyCallback callback);
    CallbackId addKeyCallback(Key key, KeyState state, KeyCallbackNoDt callback);
    CallbackId addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                      MouseButtonCallback callback);
    CallbackId addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                      MouseButtonCallbackNoDt callback);
    CallbackId addMouseMoveCallback(MouseMoveCallback callback);
    CallbackId addMouseMoveCallback(MouseMoveCallbackNoDt callback);
    CallbackId addMouseScrollCallback(MouseScrollCallback callback);
    CallbackId addMouseScrollCallback(MouseScrollCallbackNoDt callback);
    CallbackId setCloseCallback(CloseCallback callback);

    void removeCallback(CallbackId id);
    void removeCallbacks(const std::span<CallbackId> &ids);

    void update() const;
    void clear() const;
    void swapBuffers() const;
    void setCursorState(CursorState state) const;
    int getWidth() const;
    int getHeight() const;
    float getAspect() const;

  private:
    static Window *getUserPointer(GLFWwindow *windowHandle);

    constexpr static CallbackId CLOSE_CALLBACK_ID = 0;

    static inline int s_nWindows              = 0;
    static inline CallbackId s_lastCallbackId = 0;

    GLFWwindow *m_windowHandle = nullptr;
    std::string m_name;
    int m_width;
    int m_height;
    float m_lastDt;

    std::array<std::vector<KeyCallbackItem>, NUM_KEYS> m_keyCallbacks;
    std::array<std::vector<MouseButtonCallbackItem>, NUM_MOUSE_BUTTONS> m_mouseButtonCallbacks;
    std::vector<MouseMoveCallbackItem> m_mouseMoveCallbacks;
    std::vector<MouseScrollCallbackItem> m_mouseScrollCallbacks;
    CloseCallback m_closeCallback;

    std::vector<int> m_activeKeys;
    std::vector<int> m_activeMouseButtons;
  };
} // namespace TritiumEngine::Rendering