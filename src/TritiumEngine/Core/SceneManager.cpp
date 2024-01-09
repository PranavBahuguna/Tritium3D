#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Core/SceneManager.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  SceneManager::SceneManager(Application *app) : m_sceneIt(m_scenes.end()), m_app(app) {}

  /**
   * @brief Registers a new scene with the app
   * @param scene Pointer to the scene to be registered
   */
  void SceneManager::addScene(std::unique_ptr<Scene> scene) {
    m_scenes.emplace_back(std::move(scene));
    if (m_scenes.size() == 1)
      m_sceneIt = m_scenes.begin();

    m_scenes.back()->registerWithApplication(*m_app);
  }

  /**
   * @brief Loads the next scene
   * @param rotate If enabled, loads the first scene if on the last scene. If disabled, this stops
   * the application instead.
   */
  void SceneManager::nextScene(bool rotate) {
    auto it = m_sceneIt + 1;
    if (it == m_scenes.end()) {
      if (!rotate) {
        m_app->stop();
        return;
      } else {
        it = m_scenes.begin();
      }
    }

    loadScene(it);
  }

  /**
   * @brief Changes to a previous scene
   * @param rotate If enabled, loads the last scene if on the first scene. If disabled, this does
   * nothing.
   */
  void SceneManager::prevScene(bool rotate) {
    if (m_sceneIt == m_scenes.begin() && !rotate)
      return;

    auto it = m_sceneIt != m_scenes.begin() ? m_sceneIt - 1 : m_scenes.end() - 1;
    loadScene(it);
  }

  /**
   * @brief Loads a scene given its index
   * @param index The index of the scene to load
   */
  void SceneManager::loadScene(size_t index) {
    if (index >= m_scenes.size())
      Logger::warn("[SceneManager] Scene index {} is out of range", index);
    else
      loadScene(m_scenes.begin() + index);
  }

  /**
   * @brief Loads a scene given its name
   * @param name The name of the scene to load
   */
  void SceneManager::loadScene(const std::string &name) {
    auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
                           [&name](const auto &scene) { return scene->name == name; });
    if (it == m_scenes.end())
      Logger::warn("[SceneManager] Scene '{}' could not be found", name);
    else
      loadScene(it);
  }

  /** @brief Reloads the current scene */
  void SceneManager::reloadCurrentScene() { loadScene(m_sceneIt); }

  /** @brief Updates the current scene */
  void SceneManager::update(float dt) const { (*m_sceneIt)->update(dt); }

  /** @brief Determines if the manager has any registered scenes */
  bool SceneManager::hasScenes() const { return !m_scenes.empty(); }

  /** @brief Obtains the current scene's entity */
  entt::entity SceneManager::getCurrentSceneEntity() const { return (*m_sceneIt)->getEntity(); }

  void SceneManager::loadScene(SceneList::iterator it) {
    // Unloads current scene iterator and loads new one
    if (m_sceneIt != m_scenes.end())
      (*m_sceneIt)->unload();
    m_sceneIt = it;
    (*m_sceneIt)->load();
  }

} // namespace TritiumEngine::Core