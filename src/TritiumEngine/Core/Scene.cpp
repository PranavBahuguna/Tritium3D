#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Core/Scriptable.hpp>
#include <TritiumEngine/Core/System.hpp>

namespace TritiumEngine::Core
{
  Scene::Scene(const std::string &name) : name(name) {}

  Scene::~Scene() = default;

  /**
   * @brief Performs scene setup with the given application
   * @param app The application to register this scene with
   */
  void Scene::registerWithApplication(Application &app) {
    m_app = &app;
    onRegister();
  }

  /** @brief Initialises the scene and all constituent systems */
  void Scene::load() {
    Logger::info("[Scene] Loading scene '{}'...", name);
    m_app->registry.view<NativeScript>().each(
        [&](auto entity, NativeScript &script) { script.getInstance().init(); });
    init();
  }

  /** @brief Clears the ECS registry, dispatcher and any registered systems */
  void Scene::unload() {
    Logger::info("[Scene] Unloading scene '{}'...", name);
    m_app->registry.view<NativeScript>().each(
        [&](auto entity, NativeScript &script) { script.getInstance().dispose(); });
    dispose();

    m_systems.clear();
    m_app->registry.clear();
    m_app->dispatcher.clear();
  }

  /**
   * @brief Updates all active systems and scripts
   * @param dt Time delta since last frame
   */
  void Scene::update(float dt) {
    for (auto &system : m_systems)
      system->update(dt);

    m_app->registry.view<NativeScript>().each([&](auto entity, NativeScript &script) {
      if (script.getInstance().isEnabled())
        script.getInstance().update(dt);
    });
    onUpdate(dt);
  }
} // namespace TritiumEngine::Core