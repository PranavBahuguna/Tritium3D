#pragma once

namespace TritiumEngine::Core
{
  class Application;

  class System {
  public:
    virtual void update(float dt) {}

    void setup(Application &app) { m_app = &app; }

  protected:
    Application *m_app;
  };
} // namespace TritiumEngine::Core
