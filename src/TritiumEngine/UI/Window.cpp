#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/UI/Window.hpp>

#include <stdexcept>

using namespace TritiumEngine::Input;

namespace TritiumEngine::UI
{
  Window::Window(const std::string &name, WindowMode wMode, int width, int height)
      : m_name(name), m_wMode(wMode), m_width(width), m_height(height), m_window(nullptr) {
    // Set window dimensions from screen if fullscreen enabled
    GLFWmonitor *monitor = nullptr;
    if (m_wMode == WindowMode::FULLSCREEN || m_wMode == WindowMode::FULLSCREEN_WINDOWED) {
      auto primaryMonitor = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
      m_width = mode->width;
      m_height = mode->height;

      // Use the primary monitor only if we want fullscreen without window
      if (m_wMode == WindowMode::FULLSCREEN)
        monitor = primaryMonitor;
    }

    // Try initialise window with given dimensions
    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), monitor, NULL);
    if (m_window == nullptr)
      return;

    // Set context for GLEW to use
    glfwMakeContextCurrent(m_window);

    // Setup viewport size
    glViewport(0, 0, m_width, m_height);

    // Setup keyboard handler
    glfwSetKeyCallback(m_window, Keyboard::KeyHandler);

    // Remove cursor from screen
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  Window::~Window() {
    glfwTerminate();
    glfwDestroyWindow(m_window);
  }

  /**
   * Checks if the window has been successfully initialised.
   *
   * @return True if window has been initialised.
   */
  bool Window::IsInitialised() const { return m_window != nullptr; }

  /**
   * Updates the windows status and input events, should be called every frame.
   */
  void Window::Update() const {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  /**
   * Obtain the width of the window in screen units.
   *
   * @return Window width.
   */
  int Window::GetWidth() const { return m_width; }

  /**
   * Obtain the height of the window in screen units.
   *
   * @return Window width.
   */
  int Window::GetHeight() const { return m_height; }

  /**
   * Checks if the window has been flagged to close.
   *
   * @return True if window is flagged for closing.
   */
  bool Window::GetShouldClose() const { return glfwWindowShouldClose(m_window); }

  /**
   * Sets the window flagged for closing.
   */
  void Window::Close() const { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }
} // namespace TritiumEngine::UI