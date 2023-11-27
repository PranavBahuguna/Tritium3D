#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Core/System.hpp>

namespace TritiumEngine::Core
{
  Scene::~Scene() = default;

  /**
   * @brief Performs scene setup with the given application
   * @param app The application to register this scene with
   */
  void Scene::registerWithApplication(Application &app) {
    m_app         = &app;
    m_sceneEntity = m_app->registry.create();
    for (auto &system : m_systems)
      system->Setup(*m_app);
  }

  /** @brief Initialises the scene and all constituent systems */
  void Scene::load() {
    init();
    for (auto &system : m_systems)
      system->init();
  }

  /** @brief Clears the ECS registry and dispatcher */
  void Scene::unload() {
    dispose();
    m_app->registry.clear();
    m_app->dispatcher.clear();
  }

  /**
   * @brief Registers a system to this scene
   * @param system The system to register
   */
  void Scene::addSystem(std::unique_ptr<System> system) {
    m_systems.emplace_back(std::move(system));
  }

  /**
   * @brief Updates all systems active in this scene
   * @param dt Time delta since last frame
   */
  void Scene::update(float dt) const {
    for (auto &system : m_systems)
      system->update(dt);
  }

  /**
   * @brief Obtains the entity associated with this scene
   * @return The scene entity
   */
  entt::entity Scene::getEntity() const { return m_sceneEntity; }
} // namespace TritiumEngine::Core