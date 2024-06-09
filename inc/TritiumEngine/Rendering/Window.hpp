#pragma once

#include <TritiumEngine/Rendering/Cursor.hpp>
#include <TritiumEngine/Rendering/ShaderManager.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/EnumUtils.hpp>

#include <memory>

using namespace TritiumEngine::Utilities;

struct GLFWwindow;
class FrameBuffer;

namespace TritiumEngine::Rendering
{
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
    FULLSCREEN          = FOCUSED | VISIBLE | MAXIMIZED,             // Borderless fullscreen
    FULLSCREEN_WINDOWED = DEFAULT | MAXIMIZED                        // Windowed fullscreen
  };
  ENABLE_ENUM_FLAGS(WindowHints)

  struct WindowSettings {
    std::string name;
    bool fullscreen      = false;
    WindowHints hints    = WindowHints::DEFAULT;
    int width            = 1280;
    int height           = 720;
    int aspectX          = 16;
    int aspectY          = 9;
    bool fixWindowAspect = false;
    Color clearColor     = 0xFF252525; // dark grey
    Color borderColor    = COLOR_BLACK;
  };

  class Window {
  public:
    Window(ShaderManager &shaderManager, WindowSettings settings);
    Window(const Window &window) = delete;
    Window(Window &&) noexcept   = default;
    ~Window();

    void init();
    void resize(int width, int height);

    void beginDraw() const;
    void endDraw() const;
    void clear() const;
    void swapBuffers() const;
    void setCursorState(CursorState state) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getAspect() const { return (float)m_width / m_height; }
    int getFrameWidth() const { return m_frameWidth; }
    int getFrameHeight() const { return m_frameHeight; }
    float getFrameAspect() const { return (float)m_frameAspectX / m_frameAspectY; }
    GLFWwindow *getHandle() const { return m_windowHandle; }

  private:
    static Window *getUserPointer(GLFWwindow *windowHandle);

    void calcFrameDimensions();
    void createScreenFramebuffer(float left, float right, float bottom, float top);

    static inline int s_nWindows = 0;

    GLFWwindow *m_windowHandle = nullptr;

    std::string m_name;
    bool m_fullscreen;
    int m_width;
    int m_height;
    int m_frameWidth;
    int m_frameHeight;
    int m_frameAspectX;
    int m_frameAspectY;
    Color m_clearColor;
    Color m_borderColor;

    ShaderManager &m_shaderManager;
    std::unique_ptr<FrameBuffer> m_frameBuffer;
    unsigned int m_screenQuadVao = 0;
    unsigned int m_screenQuadVbo = 0;
  };
} // namespace TritiumEngine::Rendering