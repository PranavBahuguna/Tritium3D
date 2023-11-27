#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entity/entity.hpp>

#include <memory>
#include <vector>

namespace TritiumEngine::Core
{
  class Application;
  class System;

  class Scene {
  public:
    ~Scene();

    virtual void init()    = 0;
    virtual void dispose() = 0;

    void registerWithApplication(Application &engine);
    void load();
    void unload();
    void addSystem(std::unique_ptr<System> system);

    void update(float dt) const;
    entt::entity getEntity() const;

  private:
    Application *m_app;
    entt::entity m_sceneEntity;
    std::vector<std::unique_ptr<System>> m_systems;
  };
} // namespace TritiumEngine::Core

#endif // SCENE_HPP
