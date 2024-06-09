#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Input
{
  InputManager::InputManager(GLFWwindow *windowHandle) : m_windowHandle(windowHandle) {
    s_handleMap[m_windowHandle] = this;
  }

  /** @brief Updates input events, should be called every frame */
  void InputManager::update(float dt) {
    glfwPollEvents();
    m_lastDt = dt;

    // Run callbacks for continuously pressed keys or mouse buttons
    for (int key : m_activeKeys) {
      for (const auto &callbackItem : m_keyCallbacks[key]) {
        if (innerType(callbackItem.state) == innerType(KeyState::PRESSED))
          callbackItem.callback(dt);
      }
    }
    for (int button : m_activeMouseButtons) {
      for (const auto &callbackItem : m_mouseButtonCallbacks[button]) {
        if (innerType(callbackItem.state) == innerType(MouseButtonState::PRESSED))
          callbackItem.callback(dt);
      }
    }
  }

  /**
   * @brief Adds a callback action for keyboard input
   * @param key The keyboard key to add a callback for
   * @param state The state of the key to trigger the callback on
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addKeyCallback(Key key, KeyState state, KeyCallback callback) {
    // Add action to callbacks
    m_keyCallbacks[innerType(key)].emplace_back(state, std::move(callback), ++s_lastCallbackId);

    glfwSetKeyCallback(m_windowHandle,
                       [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
                         const auto &input = s_handleMap[windowHandle];
                         auto &activeKeys  = input->m_activeKeys;

                         // Update active keys
                         if (action == GLFW_PRESS)
                           activeKeys.push_back(key);
                         else if (action == GLFW_RELEASE)
                           activeKeys.erase(std::remove(activeKeys.begin(), activeKeys.end(), key));

                         // Run all callbacks for the given key
                         for (const auto &callbackItem : input->m_keyCallbacks[key]) {
                           if (action == innerType(callbackItem.state)) // exclude KeyState::PRESSED
                             callbackItem.callback(input->m_lastDt);
                         }
                       });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for keyboard input
   * @param key The keyboard key to add a callback for
   * @param state The state of the key to trigger the callback on
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addKeyCallback(Key key, KeyState state, KeyCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addKeyCallback(key, state, [callback](float) { callback(); });
  }

  /**
   * @brief Adds a callback action for mouse button input
   * @param button The mouse button to add a callback for
   * @param state The state of the button to trigger the callback on
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                                  MouseButtonCallback callback) {
    // Add action to callbacks
    m_mouseButtonCallbacks[innerType(button)].emplace_back(state, std::move(callback),
                                                           ++s_lastCallbackId);

    glfwSetMouseButtonCallback(
        m_windowHandle, [](GLFWwindow *windowHandle, int button, int action, int mods) {
          const auto &input   = s_handleMap[windowHandle];
          auto &activeButtons = input->m_activeMouseButtons;

          // Update active keys
          if (action == GLFW_PRESS)
            activeButtons.push_back(button);
          else if (action == GLFW_RELEASE)
            activeButtons.erase(std::remove(activeButtons.begin(), activeButtons.end(), button));

          // Run all callbacks for the given mouse button
          for (const auto &callbackItem : input->m_mouseButtonCallbacks[button]) {
            if (action == innerType(callbackItem.state)) // exclude MouseButtonState::PRESSED
              callbackItem.callback(input->m_lastDt);
          }
        });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse button input
   * @param button The mouse button to add a callback for
   * @param state The state of the button to trigger the callback on
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                                  MouseButtonCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseButtonCallback(button, state, [callback](float) { callback(); });
  }

  /**
   * @brief Adds a callback action for mouse movement input
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseMoveCallback(MouseMoveCallback callback) {
    // Add action to callbacks
    m_mouseMoveCallbacks.emplace_back(std::move(callback), 0.l, 0.l, true, ++s_lastCallbackId);

    glfwSetCursorPosCallback(m_windowHandle,
                             [](GLFWwindow *windowHandle, double xPos, double yPos) {
                               const auto &input = s_handleMap[windowHandle];

                               // Run all mouse movement callbacks
                               for (auto &callbackItem : input->m_mouseMoveCallbacks) {
                                 // Set prev x/y if moving mouse for first time
                                 if (callbackItem.mouseFirstMove) {
                                   callbackItem.prevXPos       = xPos;
                                   callbackItem.prevYPos       = yPos;
                                   callbackItem.mouseFirstMove = false;
                                   continue;
                                 }

                                 // Calculate change in x/y positions
                                 double deltaX = xPos - callbackItem.prevXPos;
                                 double deltaY = yPos - callbackItem.prevYPos;
                                 callbackItem.callback(input->m_lastDt, deltaX, deltaY);

                                 // Update old x/y positions
                                 callbackItem.prevXPos = xPos;
                                 callbackItem.prevYPos = yPos;
                               }
                             });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse movement input
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseMoveCallback(MouseMoveCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseMoveCallback([callback](float, double x, double y) { callback(x, y); });
  }

  /**
   * @brief Adds a callback action for mouse scroll input
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseScrollCallback(MouseScrollCallback callback) {
    // Add action to callbacks
    m_mouseScrollCallbacks.emplace_back(std::move(callback), ++s_lastCallbackId);

    glfwSetScrollCallback(m_windowHandle,
                          [](GLFWwindow *windowHandle, double xOffset, double yOffset) {
                            // Run all mouse scroll callbacks
                            const auto &input = s_handleMap[windowHandle];
                            for (const auto &callbackItem : input->m_mouseScrollCallbacks)
                              callbackItem.callback(input->m_lastDt, xOffset, yOffset);
                          });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse scroll input
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId InputManager::addMouseScrollCallback(MouseScrollCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseScrollCallback([callback](float, double x, double y) { callback(x, y); });
  }

  /**
   * @brief Adds a callback acion to trigger on window close
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId InputManager::setCloseCallback(CloseCallback callback) {
    // Set the close callback
    m_closeCallback = std::move(callback);

    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *windowHandle) {
      // Run the close callback
      const auto &input = s_handleMap[windowHandle];
      input->m_closeCallback();
    });

    return CLOSE_CALLBACK_ID;
  }

  /**
   * @brief Removes a callback given its id
   * @param id Id referencing the callback to be removed
   */
  void InputManager::removeCallback(CallbackId id) {
    if (id == CLOSE_CALLBACK_ID)
      return;

    // Check key callbacks
    for (auto &callbacks : m_keyCallbacks) {
      auto it = std::find_if(callbacks.begin(), callbacks.end(),
                             [id](const KeyCallbackItem &record) { return id == record.id; });
      if (it != callbacks.end()) {
        callbacks.erase(it);
        return;
      }
    }

    // Check mouse button callbacks
    for (auto &callbacks : m_mouseButtonCallbacks) {
      auto it =
          std::find_if(callbacks.begin(), callbacks.end(),
                       [id](const MouseButtonCallbackItem &record) { return id == record.id; });
      if (it != callbacks.end()) {
        callbacks.erase(it);
        return;
      }
    }

    // Check mouse move callbacks
    auto mouseMoveIt =
        std::find_if(m_mouseMoveCallbacks.begin(), m_mouseMoveCallbacks.end(),
                     [id](const MouseMoveCallbackItem &record) { return id == record.id; });
    if (mouseMoveIt != m_mouseMoveCallbacks.end()) {
      m_mouseMoveCallbacks.erase(mouseMoveIt);
      return;
    }

    // Check mouse scroll callbacks
    auto mouseScrollIt =
        std::find_if(m_mouseScrollCallbacks.begin(), m_mouseScrollCallbacks.end(),
                     [id](const MouseScrollCallbackItem &record) { return id == record.id; });
    if (mouseScrollIt != m_mouseScrollCallbacks.end()) {
      m_mouseScrollCallbacks.erase(mouseScrollIt);
      return;
    }

    Logger::warn("[InputManager] Could not remove callback with id {}.", id);
  }

  /**
   * @brief Removes multiple callbacks with a collection of ids
   * @param ids Ids referencing the callbacks to be removed
   */
  void InputManager::removeCallbacks(const std::span<CallbackId> &ids) {
    for (CallbackId id : ids)
      removeCallback(id);
  }
} // namespace TritiumEngine::Input