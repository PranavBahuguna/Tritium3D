#pragma once

#include <TritiumEngine/Utilities/Logger.hpp>

using namespace TritiumEngine::Utilities;

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
    void update(float dt);

    /**
     * @brief Registers a new system
     * @tparam T Type of system to register
     * @param Args Additional parameters required to construct the system
     */
    template <class T, typename... Args, IsSystemType<T> = true> void addSystem(Args &&...args) {
      if (m_app == nullptr) {
        Logger::error("[Scene] Cannot add system {} before scene is registered with application.",
                      typeid(T).name());
        return;
      }

      if (hasSystem<T>()) {
        Logger::warn("[Scene] System {} is already registered with this scene.", typeid(T).name());
        return;
      }

      m_systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
      m_systems.back()->setup(*m_app);
      m_systems.back()->init();
    }

    /**
     * @brief Unregisters an existing system
     * @tparam T Type of system to remove
     */
    template <class T, IsSystemType<T> = true> void removeSystem() {
      auto it = std::find_if(m_systems.begin(), m_systems.end(),
                             [&](const auto &s) { return dynamic_cast<T *>(s.get()) != nullptr; });
      if (it == m_systems.end()) {
        Logger::warn("[Scene] Could not remove system {} from this scene.", typeid(T).name());
        return;
      }

      (*it)->dispose();
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
    virtual void onRegister() {}
    virtual void onUpdate(float dt) {}
    virtual void init() {}
    virtual void dispose() {}

    Application *m_app = nullptr;

  private:
    std::vector<std::unique_ptr<System>> m_systems;
  };
} // namespace TritiumEngine::Core