#pragma once

#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Input/Mouse.hpp>

#include <array>
#include <functional>
#include <span>

using namespace TritiumEngine::Input::Keyboard;
using namespace TritiumEngine::Input::Mouse;

namespace TritiumEngine::Input
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

  class InputManager {
  public:
    InputManager(GLFWwindow *windowHandle);

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

  private:
    constexpr static CallbackId CLOSE_CALLBACK_ID = 0;

    static inline CallbackId s_lastCallbackId = 0;
    static inline std::unordered_map<GLFWwindow *, InputManager *> s_handleMap;

    GLFWwindow *m_windowHandle;

    std::array<std::vector<KeyCallbackItem>, NUM_KEYS> m_keyCallbacks;
    std::array<std::vector<MouseButtonCallbackItem>, NUM_MOUSE_BUTTONS> m_mouseButtonCallbacks;
    std::vector<MouseMoveCallbackItem> m_mouseMoveCallbacks;
    std::vector<MouseScrollCallbackItem> m_mouseScrollCallbacks;
    CloseCallback m_closeCallback;

    float m_lastDt;
    std::vector<int> m_activeKeys;
    std::vector<int> m_activeMouseButtons;
  };
} // namespace TritiumEngine::Input