#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entity/entity.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace TritiumEngine::Core
{
  class Application;
  class System;

  template <typename T>
  using IsSystemType = typename std::enable_if_t<std::is_base_of_v<System, T>, bool>;

  class Scene {
  public:
    Scene(const std::string &name);
    ~Scene();

    void registerWithApplication(Application &app);
    void load();
    void unload();

    void update(float dt) const;
    entt::entity getEntity() const;

    /**
     * @brief Registers a new system
     * @tparam T Type of system to register
     * @tparam Args Additional parameters required to construct the system
     */
    template <class T, typename... Args, IsSystemType<T> = true> void addSystem(Args &&...args) {
      if (!hasSystem<T>())
        m_systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief Unregisters an existing system
     * @tparam T Type of system to remove
     */
    template <class T, IsSystemType<T> = true> void removeSystem() {
      auto it = std::find_if(m_systems.begin(), m_systems.end(),
                             [&](const auto &s) { return dynamic_cast<T *>(s.get()) != nullptr; });
      if (it != m_systems.end())
        m_systems.erase(it);
    }

    /**
     * @brief Determines if a system of type T is currently registered
     * @tparam T The system type to find
     * @return True if system type T is registered, false otherwise
     */
    template <class T, IsSystemType<T> = true> bool hasSystem() const {
      for (auto &system : m_systems) {
        if (dynamic_cast<T *>(system.get()))
          return true;
      }
      return false;
    }

    /**
     * @brief Tries obtaining a system of type T from the registered systems
     * @tparam T The system type to obtain
     * @return Pointer to system if successful, nullptr otherwise
     */
    template <class T, IsSystemType<T> = true> T *getSystem() const {
      for (auto &system : m_systems) {
        if (T *cast = dynamic_cast<T *>(system.get()))
          return cast;
      }
      return nullptr;
    }

    const std::string name;

  protected:
    virtual void init() {}
    virtual void dispose() {}

    Application *m_app;

  private:
    entt::entity m_sceneEntity;
    std::vector<std::unique_ptr<System>> m_systems;
  };
} // namespace TritiumEngine::Core

#endif // SCENE_HPP
