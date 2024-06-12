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
      : m_name(settings.name), m_fullscreen(settings.fullscreen), m_width(settings.width),
        m_height(settings.height), m_clearColor(settings.clearColor),
        m_borderColor(settings.borderColor), m_frameWidth(settings.width),
        m_frameHeight(settings.height), m_frameAspectX(settings.aspectX),
        m_frameAspectY(settings.aspectY), m_shaderManager(shaderManager) {
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

    // Get settings for fullscreen mode
    GLFWmonitor *monitor = nullptr;
    if (m_fullscreen) {
      monitor               = glfwGetPrimaryMonitor();
      const auto *videoMode = glfwGetVideoMode(monitor);
      m_width               = videoMode->width;
      m_height              = videoMode->height;
      m_frameWidth          = m_width;
      m_frameHeight         = m_height;
    }

    for (auto &[major, minor] : glVersions) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

      m_windowHandle =
          glfwCreateWindow(m_width, m_height, m_name.c_str(), monitor, glfwGetCurrentContext());
      if (m_windowHandle != nullptr) {
        Logger::info("[Window] OpenGL version {}.{} found.", major, minor);
        break;
      }
    }

    if (m_windowHandle == nullptr)
      throw std::runtime_error("Error: Window creation failed.");

    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwMakeContextCurrent(m_windowHandle);

    if (!m_fullscreen) {
      // Obtain actual window / frame dimensions
      glfwGetFramebufferSize(m_windowHandle, &m_width, &m_height);
      calcFrameDimensions();

      if (settings.fixWindowAspect)
        glfwSetWindowAspectRatio(m_windowHandle, settings.aspectX, settings.aspectY);

      glfwSetFramebufferSizeCallback(m_windowHandle,
                                     [](GLFWwindow *windowHandle, int width, int height) {
                                       const auto &window = getUserPointer(windowHandle);
                                       window->resize(width, height);
                                     });
    }

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
  void Window::init() {
    if (m_fullscreen) {
      glViewport(0, 0, m_width, m_height);
      createScreenFramebuffer(-1.f, 1.f, -1.f, 1.f);
    } else {
      resize(m_width, m_height);
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
    calcFrameDimensions();

    float left   = (float)-m_frameWidth / m_width;
    float right  = (float)m_frameWidth / m_width;
    float bottom = (float)-m_frameHeight / m_height;
    float top    = (float)m_frameHeight / m_height;

    glViewport(0, 0, m_width, m_height);
    createScreenFramebuffer(left, right, bottom, top);
  }

  void Window::beginDraw() const {
    m_frameBuffer->bind();
    m_frameBuffer->clear(m_clearColor);
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

  Window *Window::getUserPointer(GLFWwindow *windowHandle) {
    return static_cast<Window *>(glfwGetWindowUserPointer(windowHandle));
  }

  void Window::calcFrameDimensions() {
    // Obtain frame width / height for a given frame aspect ratio for non-fullscreen windows
    float windowAspect = getAspect();
    float frameAspect  = getFrameAspect();

    if (windowAspect > frameAspect) {
      m_frameHeight = m_height - (m_height % m_frameAspectY);
      m_frameWidth  = (m_frameHeight / m_frameAspectY) * m_frameAspectX;
    } else {
      m_frameWidth  = m_width - (m_width % m_frameAspectX);
      m_frameHeight = (m_frameWidth / m_frameAspectX) * m_frameAspectY;
    }
  }

  void Window::createScreenFramebuffer(float left, float right, float bottom, float top) {
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

} // namespace TritiumEngine::Rendering