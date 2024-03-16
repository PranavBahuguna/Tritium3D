#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <array>
#include <stdexcept>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  Window::Window(WindowSettings settings)
      : m_name(settings.name), m_width(settings.width), m_height(settings.height) {
    // Init GLFW library if not already done so
    if (s_nWindows == 0) {
      if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("Error: GLFW could not be initialised.");

      glfwSetErrorCallback([](int errorCode, const char *description) {
        Logger::error("[GLFW] {} (error code: {}).", description, errorCode);
      });
    }

    // Apply any window settings
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_FOCUSED, innerType(settings.hints & WindowHints::FOCUSED));
    glfwWindowHint(GLFW_RESIZABLE, innerType(settings.hints & WindowHints::RESIZABLE));
    glfwWindowHint(GLFW_VISIBLE, innerType(settings.hints & WindowHints::VISIBLE));
    glfwWindowHint(GLFW_DECORATED, innerType(settings.hints & WindowHints::DECORATED));
    glfwWindowHint(GLFW_AUTO_ICONIFY, innerType(settings.hints & WindowHints::AUTO_MINIMIZE));
    glfwWindowHint(GLFW_FLOATING, innerType(settings.hints & WindowHints::ALWAYS_ON_TOP));
    glfwWindowHint(GLFW_MAXIMIZED, innerType(settings.hints & WindowHints::MAXIMIZED));
    glfwWindowHint(GLFW_CENTER_CURSOR, innerType(settings.hints & WindowHints::CENTER_CURSOR));
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   innerType(settings.hints & WindowHints::TRANSPARENT_FB));
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, innerType(settings.hints & WindowHints::AUTOFOCUS));

    // Obtain the latest compatible OpenGL version available
    static constexpr std::array<std::pair<int, int>, 8> glVersions = {
        {{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}}};

    for (auto &[major, minor] : glVersions) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

      m_windowHandle =
          glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, glfwGetCurrentContext());

      if (m_windowHandle != nullptr) {
        Logger::info("[Window] OpenGL version {}.{} found.", major, minor);
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
    Logger::info("[Window] Opened window '{}'.", m_name);
  }

  Window::~Window() {
    if (m_windowHandle == nullptr)
      return;

    glfwDestroyWindow(m_windowHandle);

    // Terminate GLFW library if there are no windows left
    if (--s_nWindows == 0)
      glfwTerminate();

    Logger::info("[Window] Closed window '{}'", m_name);
  }

  /** @brief Updates the windows status and input events, should be called every frame */
  void Window::update() const {
    glfwSwapBuffers(m_windowHandle);
    glfwPollEvents();
  }

  /** @brief Clears the window and any buffer bits */
  void Window::refresh() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  }

  /**
   * @brief Sets the current state of the cursor
   * @param state The new cursor state
   */
  void Window::setCursorState(CursorState state) const {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, innerType(state));
  }

  /** @brief Obtains window width in pixels */
  int Window::getWidth() const { return m_width; }

  /** @brief Obtains window height in pixels */
  int Window::getHeight() const { return m_height; }

  /**
   * @brief Adds a callback action for keyboard input
   * @param key The keyboard key to add a callback for
   * @param state The state of the key to trigger the callback on
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::addKeyCallback(Key key, KeyState state, KeyCallback callback) {
    // Add action to callbacks
    m_keyCallbacks[innerType(key)].emplace_back(state, std::move(callback), ++s_lastCallbackId);

    glfwSetKeyCallback(m_windowHandle,
                       [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
                         const auto &window = getUserPointer(windowHandle);

                         // Run all callbacks for the given key
                         for (const auto &callbackItem : window->m_keyCallbacks[key]) {
                           if (action == innerType(callbackItem.state))
                             callbackItem.callback();
                         }
                       });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse button input
   * @param button The mouse button to add a callback for
   * @param state The state of the button to trigger the callback on
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                            MouseButtonCallback callback) {
    // Add action to callbacks
    m_mouseButtonCallbacks[innerType(button)].emplace_back(state, std::move(callback),
                                                           ++s_lastCallbackId);

    glfwSetMouseButtonCallback(
        m_windowHandle, [](GLFWwindow *windowHandle, int button, int action, int mods) {
          const auto &window = getUserPointer(windowHandle);

          // Run all callbacks for the given mouse button
          for (const auto &callbackItem : window->m_mouseButtonCallbacks[button]) {
            if (action == innerType(callbackItem.state))
              callbackItem.callback();
          }
        });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse movement input
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseMoveCallback(MouseMoveCallback callback) {
    // Add action to callbacks
    m_mouseMoveCallbacks.emplace_back(std::move(callback), ++s_lastCallbackId);

    glfwSetCursorPosCallback(m_windowHandle,
                             [](GLFWwindow *windowHandle, double xpos, double ypos) {
                               const auto &window = getUserPointer(windowHandle);

                               // Run all mouse movement callbacks
                               for (const auto &callbackItem : window->m_mouseMoveCallbacks)
                                 callbackItem.callback(xpos, ypos);
                             });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse scroll input
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseScrollCallback(MouseScrollCallback callback) {
    // Add action to callbacks
    m_mouseScrollCallbacks.emplace_back(std::move(callback), ++s_lastCallbackId);

    glfwSetScrollCallback(m_windowHandle, [](GLFWwindow *windowHandle, double xPos, double yPos) {
      const auto &window = getUserPointer(windowHandle);

      // Run all mouse scroll callbacks
      for (const auto &callbackItem : window->m_mouseScrollCallbacks)
        callbackItem.callback(xPos, yPos);
    });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback acion to trigger on window close
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::setCloseCallback(CloseCallback callback) {
    // Set the close callback
    m_closeCallback = std::move(callback);

    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *windowHandle) {
      // Run the close callback
      const auto &window = getUserPointer(windowHandle);
      window->m_closeCallback();
    });

    return s_lastCallbackId;
  }

  /**
   * @brief Removes a callback given its id
   * @param id Id referencing the callback to be removed
   */
  void Window::removeCallback(CallbackId id) {
    if (id == 0)
      return; // null callback

    // Check key callbacks
    for (auto &callbacks : m_keyCallbacks) {
      auto it = std::find_if(callbacks.begin(), callbacks.end(),
                             [id](const KeyCallbackRecord &record) { return id == record.id; });
      if (it != callbacks.end()) {
        callbacks.erase(it);
        return;
      }
    }

    // Check mouse button callbacks
    for (auto &callbacks : m_mouseButtonCallbacks) {
      auto it =
          std::find_if(callbacks.begin(), callbacks.end(),
                       [id](const MouseButtonCallbackRecord &record) { return id == record.id; });
      if (it != callbacks.end()) {
        callbacks.erase(it);
        return;
      }
    }

    // Check mouse move callbacks
    auto mouseMoveIt =
        std::find_if(m_mouseMoveCallbacks.begin(), m_mouseMoveCallbacks.end(),
                     [id](const MouseMoveCallbackRecord &record) { return id == record.id; });
    if (mouseMoveIt != m_mouseMoveCallbacks.end()) {
      m_mouseMoveCallbacks.erase(mouseMoveIt);
      return;
    }

    // Check mouse scroll callbacks
    auto mouseScrollIt =
        std::find_if(m_mouseScrollCallbacks.begin(), m_mouseScrollCallbacks.end(),
                     [id](const MouseScrollCallbackRecord &record) { return id == record.id; });
    if (mouseScrollIt != m_mouseScrollCallbacks.end()) {
      m_mouseScrollCallbacks.erase(mouseScrollIt);
      return;
    }

    // Check close callback
    if (id == CLOSE_CALLBACK_ID) {
      m_closeCallback = {};
      return;
    }

    Logger::warn("[Window] Could not find callback to remove with id {}.", id);
  }

  /**
   * @brief Removes multiple callbacks with a collection of ids
   * @param ids Ids referencing the callbacks to be removed
   */
  void Window::removeCallbacks(const std::span<CallbackId> &ids) {
    for (CallbackId id : ids)
      removeCallback(id);
  }

  /**
   * @brief Obtains a pointer to a window given its handle
   * @param windowHandle The handle to the required window
   */
  Window *Window::getUserPointer(GLFWwindow *windowHandle) {
    return static_cast<Window *>(glfwGetWindowUserPointer(windowHandle));
  }
} // namespace TritiumEngine::Rendering