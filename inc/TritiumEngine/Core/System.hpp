#ifndef SYSTEM_HPP
#define SYSTEM_HPP

namespace TritiumEngine::Core
{
  class Application;

  class System {
  public:
    virtual void init() {}
    virtual void dispose() {}
    virtual void update(const float dt) {}

    void Setup(Application &app) { m_app = &app; }

  protected:
    Application *m_app;
  };
} // namespace TritiumEngine::Core

#endif // SYSTEM_HPP
