#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Core/Scriptable.hpp>
#include <TritiumEngine/Core/System.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <entt/entt.hpp>

namespace TritiumEngine::Core
{
  Scene::Scene(const std::string &name, Application &app) : name(name), m_app(app) {}

  Scene::~Scene() = default;

  /** @brief Initialises the scene and all constituent systems */
  void Scene::load() {
    Logger::info("[Scene] Loading scene '{}'...", name);
    init();
    m_app.registry.view<NativeScript>().each(
        [&](auto entity, NativeScript &script) { script.getInstance().init(); });
    Logger::info("[Scene] Scene '{}' loaded.", name);
  }

  /** @brief Clears the ECS registry, dispatcher and any registered systems */
  void Scene::unload() {
    Logger::info("[Scene] Unloading scene '{}'...", name);
    m_app.registry.view<NativeScript>().each(
        [&](auto entity, NativeScript &script) { script.getInstance().dispose(); });
    dispose();

    m_systems.clear();
    m_app.registry.clear();
    m_app.dispatcher.clear();
    Logger::info("[Scene] Scene '{}' unloaded.", name);
  }

  /**
   * @brief Updates all active systems and scripts
   * @param dt Time delta since last frame
   */
  void Scene::update(float dt) {
    for (auto &system : m_systems)
      system->update(dt);

    m_app.registry.view<NativeScript>().each([&](auto entity, NativeScript &script) {
      if (script.getInstance().isEnabled())
        script.getInstance().update(dt);
    });
    onUpdate(dt);
  }
} // namespace TritiumEngine::Core