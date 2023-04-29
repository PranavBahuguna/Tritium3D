#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <array>
#include <stdexcept>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  Window::Window(const std::string &name, WindowSettings settings, unsigned int width,
                 unsigned int height)
      : m_name(name), m_width(static_cast<int>(width)), m_height(static_cast<int>(height)) {
    // Init GLFW library if not already done so
    if (s_nWindows == 0) {
      if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("Error: GLFW could not be initialised.");

      glfwSetErrorCallback([](int errorCode, const char *description) {
        Logger::Error("[GLFW] {} (error code: {}).", description, errorCode);
      });
    }

    // Apply any window settings
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_FOCUSED, innerType(settings & WindowSettings::FOCUSED));
    glfwWindowHint(GLFW_RESIZABLE, innerType(settings & WindowSettings::RESIZABLE));
    glfwWindowHint(GLFW_VISIBLE, innerType(settings & WindowSettings::VISIBLE));
    glfwWindowHint(GLFW_DECORATED, innerType(settings & WindowSettings::DECORATED));
    glfwWindowHint(GLFW_AUTO_ICONIFY, innerType(settings & WindowSettings::AUTO_MINIMIZE));
    glfwWindowHint(GLFW_FLOATING, innerType(settings & WindowSettings::ALWAYS_ON_TOP));
    glfwWindowHint(GLFW_MAXIMIZED, innerType(settings & WindowSettings::MAXIMIZED));
    glfwWindowHint(GLFW_CENTER_CURSOR, innerType(settings & WindowSettings::CENTER_CURSOR));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   innerType(settings & WindowSettings::TRANSPARENT_FB));
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, innerType(settings & WindowSettings::AUTOFOCUS));

    // Obtain the latest compatible OpenGL version available
    static constexpr std::array<std::pair<int, int>, 8> glVersions = {
        {{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}}};

    for (auto &[major, minor] : glVersions) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

      m_windowHandle =
          glfwCreateWindow(m_width, m_height, name.c_str(), nullptr, glfwGetCurrentContext());

      if (m_windowHandle != nullptr) {
        Logger::Info("[Window] OpenGL {}.{} found.", major, minor);
        break;
      }
    }

    if (m_windowHandle == nullptr)
      throw std::runtime_error("Error: Window creation failed.");

    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwMakeContextCurrent(m_windowHandle);

    // Re-obtain window width/height from actual window dimensions
    glfwGetWindowSize(m_windowHandle, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);

    ++s_nWindows;
    Logger::Info("[Window] Opened window '{}'.", m_name);
  }

  Window::~Window() {
    if (m_windowHandle == nullptr)
      return;

    glfwDestroyWindow(m_windowHandle);

    // Terminate GLFW library if there are no windows left
    if (--s_nWindows == 0)
      glfwTerminate();

    Logger::Info("[Window] Closed window '{}'", m_name);
  }

  /**
   * Updates the windows status and input events, should be called every frame.
   */
  void Window::Update() const {
    glfwSwapBuffers(m_windowHandle);
    glfwPollEvents();
  }

  /**
   * Sets the current state of the cursor.
   *
   * @param state The new cursor state
   */
  void Window::SetCursorState(CursorState state) const {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, innerType(state));
  }

  /**
   * Set Window flagged for closing
   */
  void Window::SetShouldClose() const { glfwSetWindowShouldClose(m_windowHandle, GLFW_TRUE); }

  /**
   * Adds a callback action for keyboard input.
   *
   * @param key The keyboard key to add a callback for.
   * @param state The state of the key to trigger the callback on.
   * @param callback Callback action to run.
   */
  void Window::AddKeyCallback(Key key, KeyState state, KeyCallback callback) {
    // Add action to callbacks
    m_keyCallbacks[innerType(key)].emplace_back(state, std::move(callback));

    glfwSetKeyCallback(m_windowHandle,
                       [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
                         const auto &window = GetUserPointer(windowHandle);

                         // Run all callbacks for the given key
                         for (const auto &callbackItem : window->m_keyCallbacks[key]) {
                           KeyState state              = std::get<0>(callbackItem);
                           const KeyCallback &callback = std::get<1>(callbackItem);

                           if (action == innerType(state))
                             callback();
                         }
                       });
  }

  /**
   * Adds a callback action for mouse button input.
   *
   * @param button The mouse button to add a callback for.
   * @param state The state of the button to trigger the callback on.
   * @param callback Callback action to run.
   */
  void Window::AddMouseButtonCallback(MouseButton button, MouseButtonState state,
                                      MouseButtonCallback callback) {
    // Add action to callbacks
    m_mouseButtonCallbacks[innerType(button)].emplace_back(state, std::move(callback));

    glfwSetMouseButtonCallback(
        m_windowHandle, [](GLFWwindow *windowHandle, int button, int action, int mods) {
          const auto &window = GetUserPointer(windowHandle);

          // Run all callbacks for the given mouse button
          for (const auto &callbackItem : window->m_mouseButtonCallbacks[button]) {
            MouseButtonState state              = std::get<0>(callbackItem);
            const MouseButtonCallback &callback = std::get<1>(callbackItem);

            if (action == innerType(state))
              callback();
          }
        });
  }

  /**
   * Adds a callback action for mouse movement input.
   *
   * @param callback Callback action to run.
   */
  void Window::AddMouseMoveCallback(MouseMoveCallback callback) {
    // Add action to callbacks
    m_mouseMoveCallbacks.emplace_back(std::move(callback));

    glfwSetCursorPosCallback(
        m_windowHandle, [](GLFWwindow *windowHandle, double xpos, double ypos) {
          const auto &window = GetUserPointer(windowHandle);

          // Run all mouse movement callbacks
          for (const MouseMoveCallback &callback : window->m_mouseMoveCallbacks)
            callback(xpos, ypos);
        });
  }

  /**
   * Adds a callback action for mouse scroll input.
   *
   * @param callback Callback action to run.
   */
  void Window::AddMouseScrollCallback(MouseScrollCallback callback) {
    // Add action to callbacks
    m_mouseScrollCallbacks.emplace_back(std::move(callback));

    glfwSetScrollCallback(m_windowHandle, [](GLFWwindow *windowHandle, double xPos, double yPos) {
      const auto &window = GetUserPointer(windowHandle);

      // Run all mouse scroll callbacks
      for (const MouseScrollCallback &callback : window->m_mouseScrollCallbacks)
        callback(xPos, yPos);
    });
  }

  int Window::GetWidth() const { return m_width; }

  int Window::GetHeight() const { return m_height; }

  int Window::GetShouldClose() const { return glfwWindowShouldClose(m_windowHandle); }

  /**
   * Obtains a pointer to a window given its handle.
   *
   * @param windowHandle The handle to the required window.
   */
  Window *Window::GetUserPointer(GLFWwindow *windowHandle) {
    return static_cast<Window *>(glfwGetWindowUserPointer(windowHandle));
  }
} // namespace TritiumEngine::Rendering