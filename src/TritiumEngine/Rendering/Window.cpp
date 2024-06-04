#include <TritiumEngine/Rendering/Components/FrameBuffer.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <array>
#include <stdexcept>

namespace TritiumEngine::Rendering
{
  using TextureAttachment = FrameBuffer::TextureAttachment;
  using BufferAttachment  = FrameBuffer::BufferAttachment;

  Window::Window(ShaderManager &shaderManager, WindowSettings settings)
      : m_name(settings.name), m_width(settings.width), m_height(settings.height),
        m_clearColor(settings.clearColor), m_borderColor(settings.borderColor),
        m_frameAspect((float)settings.aspectX / settings.aspectY), m_lastDt(0.f),
        m_shaderManager(shaderManager) {
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

    // Window sizing settings
    glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *windowHandle, int width, int height) {
      const auto &window = getUserPointer(windowHandle);
      window->resize(width, height);
    });
    if (settings.fixWindowSizeToAspect)
      glfwSetWindowAspectRatio(m_windowHandle, settings.aspectX, settings.aspectY);

    // Re-obtain window width/height from actual dimensions
    glfwGetWindowSize(m_windowHandle, &m_width, &m_height);

    ++s_nWindows;
    Logger::info("[Window] Opened window '{}'.", m_name);
  }

  Window::~Window() {
    if (m_windowHandle == nullptr)
      return;

    // Delete screen quad data
    glDeleteVertexArrays(1, &m_screenQuadVao);
    glDeleteBuffers(1, &m_screenQuadVbo);

    glfwDestroyWindow(m_windowHandle);

    // Terminate GLFW library if there are no windows left
    if (--s_nWindows == 0)
      glfwTerminate();

    Logger::info("[Window] Closed window '{}'", m_name);
  }

  /** @brief Performs additional initialization for the window once application is ready */
  void Window::init() { resize(m_width, m_height); }

  /** @brief Updates the windows status and input events, should be called every frame */
  void Window::update(float dt) {
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
   * @brief Handles window resize and regenerates framebuffer
   * @param width New screen width
   * @param height New screen height
   */
  void Window::resize(int width, int height) {
    m_width  = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);

    float windowAspect    = getAspect();
    float horizontalRatio = std::min(m_frameAspect / windowAspect, 1.f);
    float verticalRatio   = std::min(windowAspect / m_frameAspect, 1.f);

    float left    = -horizontalRatio;
    float right   = horizontalRatio;
    float bottom  = -verticalRatio;
    float top     = verticalRatio;
    m_frameWidth  = int(horizontalRatio * width);
    m_frameHeight = int(verticalRatio * height);

    // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates
    float screenQuadData[] = {
        // positions   // texCoords
        left,  top,    0.0f, 1.0f, // T1 - 1
        left,  bottom, 0.0f, 0.0f, // T1 - 2
        right, bottom, 1.0f, 0.0f, // T1 - 3
        left,  top,    0.0f, 1.0f, // T2 - 1
        right, bottom, 1.0f, 0.0f, // T2 - 2
        right, top,    1.0f, 1.0f  // T2 - 3
    };

    // Delete old screen quad vao/vbo if previously created
    glDeleteVertexArrays(1, &m_screenQuadVao);
    glDeleteBuffers(1, &m_screenQuadVbo);

    // Create screen quad
    glGenVertexArrays(1, &m_screenQuadVao);
    glBindVertexArray(m_screenQuadVao);

    glGenBuffers(1, &m_screenQuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadData), &screenQuadData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // Create framebuffer color texture
    auto colorTexture = std::make_unique<Texture>(m_width, m_height, GL_TEXTURE_2D, GL_RGB, GL_RGB,
                                                  GL_UNSIGNED_BYTE);
    colorTexture->bind();
    colorTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    colorTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    colorTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    colorTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    colorTexture->unbind();

    // Create framebuffer with color and depth/stencil attachments
    m_frameBuffer = std::make_unique<FrameBuffer>(m_width, m_height);
    m_frameBuffer->attachTexture(TextureAttachment::COLOR, std::move(colorTexture));
    m_frameBuffer->attachRenderBuffer(BufferAttachment::DEPTH_STENCIL);
    if (!m_frameBuffer->isComplete())
      throw std::runtime_error("Error: Window framebuffer is not complete.");

    // Set screen texture
    m_shaderManager.use("screen");
    m_shaderManager.setInt("screenTexture", 0);
  }

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
                         auto &activeKeys   = window->m_activeKeys;

                         // Update active keys
                         if (action == GLFW_PRESS)
                           activeKeys.push_back(key);
                         else if (action == GLFW_RELEASE)
                           activeKeys.erase(std::remove(activeKeys.begin(), activeKeys.end(), key));

                         // Run all callbacks for the given key
                         for (const auto &callbackItem : window->m_keyCallbacks[key]) {
                           if (action == innerType(callbackItem.state)) // exclude KeyState::PRESSED
                             callbackItem.callback(window->m_lastDt);
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
  CallbackId Window::addKeyCallback(Key key, KeyState state, KeyCallbackNoDt callback) {
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
  CallbackId Window::addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                            MouseButtonCallback callback) {
    // Add action to callbacks
    m_mouseButtonCallbacks[innerType(button)].emplace_back(state, std::move(callback),
                                                           ++s_lastCallbackId);

    glfwSetMouseButtonCallback(
        m_windowHandle, [](GLFWwindow *windowHandle, int button, int action, int mods) {
          const auto &window  = getUserPointer(windowHandle);
          auto &activeButtons = window->m_activeMouseButtons;

          // Update active keys
          if (action == GLFW_PRESS)
            activeButtons.push_back(button);
          else if (action == GLFW_RELEASE)
            activeButtons.erase(std::remove(activeButtons.begin(), activeButtons.end(), button));

          // Run all callbacks for the given mouse button
          for (const auto &callbackItem : window->m_mouseButtonCallbacks[button]) {
            if (action == innerType(callbackItem.state)) // exclude MouseButtonState::PRESSED
              callbackItem.callback(window->m_lastDt);
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
  CallbackId Window::addMouseButtonCallback(MouseButton button, MouseButtonState state,
                                            MouseButtonCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseButtonCallback(button, state, [callback](float) { callback(); });
  }

  /**
   * @brief Adds a callback action for mouse movement input
   * @param callback Callback action to run
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseMoveCallback(MouseMoveCallback callback) {
    // Add action to callbacks
    m_mouseMoveCallbacks.emplace_back(std::move(callback), 0.l, 0.l, true, ++s_lastCallbackId);

    glfwSetCursorPosCallback(m_windowHandle,
                             [](GLFWwindow *windowHandle, double xPos, double yPos) {
                               const auto &window = getUserPointer(windowHandle);

                               // Run all mouse movement callbacks
                               for (auto &callbackItem : window->m_mouseMoveCallbacks) {
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
                                 callbackItem.callback(window->m_lastDt, deltaX, deltaY);

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
  CallbackId Window::addMouseMoveCallback(MouseMoveCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseMoveCallback([callback](float, double x, double y) { callback(x, y); });
  }

  /**
   * @brief Adds a callback action for mouse scroll input
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseScrollCallback(MouseScrollCallback callback) {
    // Add action to callbacks
    m_mouseScrollCallbacks.emplace_back(std::move(callback), ++s_lastCallbackId);

    glfwSetScrollCallback(m_windowHandle,
                          [](GLFWwindow *windowHandle, double xOffset, double yOffset) {
                            const auto &window = getUserPointer(windowHandle);

                            // Run all mouse scroll callbacks
                            for (const auto &callbackItem : window->m_mouseScrollCallbacks)
                              callbackItem.callback(window->m_lastDt, xOffset, yOffset);
                          });

    return s_lastCallbackId;
  }

  /**
   * @brief Adds a callback action for mouse scroll input
   * @param callback Callback action to run (without deltaTime parameter)
   * @returns Id referencing this callback
   */
  CallbackId Window::addMouseScrollCallback(MouseScrollCallbackNoDt callback) {
    // Wrap callback in an adapter
    return addMouseScrollCallback([callback](float, double x, double y) { callback(x, y); });
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

    return CLOSE_CALLBACK_ID;
  }

  /**
   * @brief Removes a callback given its id
   * @param id Id referencing the callback to be removed
   */
  void Window::removeCallback(CallbackId id) {
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

    Logger::warn("[Window] Could not remove callback with id {}.", id);
  }

  /**
   * @brief Removes multiple callbacks with a collection of ids
   * @param ids Ids referencing the callbacks to be removed
   */
  void Window::removeCallbacks(const std::span<CallbackId> &ids) {
    for (CallbackId id : ids)
      removeCallback(id);
  }

  void Window::beginDraw() const {
    m_frameBuffer->bind();
    m_frameBuffer->clear(m_clearColor);
    glEnable(GL_DEPTH_TEST);
  }

  /** @brief Draws all rendered content to a framebuffer */
  void Window::endDraw() const {
    m_frameBuffer->unbind();
    auto borderColor = ColorUtils::ToNormalizedVec4(m_borderColor);
    glClearColor(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderManager.use("screen");
    glBindVertexArray(m_screenQuadVao);
    glDisable(GL_DEPTH_TEST);

    auto textureId = m_frameBuffer->getTextureAttachment(TextureAttachment::COLOR)->getId();
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  /** @brief Clears the window and any buffer bits */
  void Window::clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

  /** @brief Swaps render buffers for this window */
  void Window::swapBuffers() const { glfwSwapBuffers(m_windowHandle); }

  /**
   * @brief Sets the current state of the cursor
   * @param state The new cursor state
   */
  void Window::setCursorState(CursorState state) const {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, innerType(state));
  }

  /**
   * @brief Obtains a pointer to a window given its handle
   * @param windowHandle The handle to the required window
   */
  Window *Window::getUserPointer(GLFWwindow *windowHandle) {
    return static_cast<Window *>(glfwGetWindowUserPointer(windowHandle));
  }
} // namespace TritiumEngine::Rendering