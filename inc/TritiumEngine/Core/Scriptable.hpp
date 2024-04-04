#pragma once

namespace TritiumEngine::Core
{
  class Application;

  class Scriptable {
  public:
    Scriptable(Application &app) : m_app(&app), m_enabled(true) {}
    virtual ~Scriptable() = default;

    bool isEnabled() const { return m_enabled; }

    void setEnabled(bool enable) {
      if (m_enabled == enable)
        return;

      m_enabled = enable;
      onEnable(enable);
    }

    void toggleEnabled() { setEnabled(!m_enabled); }

    virtual void init()           = 0;
    virtual void dispose()        = 0;
    virtual void update(float dt) = 0;

  protected:
    virtual void onEnable(bool enable) {}

    Application *m_app;
    bool m_enabled;
  };
} // namespace TritiumEngine::Core
