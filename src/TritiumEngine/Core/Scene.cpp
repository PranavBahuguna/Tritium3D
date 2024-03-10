#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Scene.hpp>
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
    m_sceneEntity = m_app->registry.create();
    init();
  }

  /** @brief Clears the ECS registry, dispatcher and any registered systems */
  void Scene::unload() {
    Logger::info("[Scene] Unloading scene '{}'...", name);
    dispose();
    for (auto &system : m_systems)
      system->dispose();

    m_systems.clear();
    m_app->registry.clear();
    m_app->dispatcher.clear();
  }

  /**
   * @brief Updates all systems active in this scene
   * @param dt Time delta since last frame
   */
  void Scene::update(float dt) {
    for (auto &system : m_systems)
      system->update(dt);

    onUpdate(dt);
  }

  /**
   * @brief Obtains the entity associated with this scene
   * @return The scene entity
   */
  entt::entity Scene::getEntity() const { return m_sceneEntity; }
} // namespace TritiumEngine::Core