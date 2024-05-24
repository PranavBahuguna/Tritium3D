#pragma once

#include <TritiumEngine/Core/Scene.hpp>

#include <entt/entity/entity.hpp>

#include <format>
#include <string>
#include <vector>

namespace TritiumEngine::Core
{
  class Application;

  using SceneList = std::vector<std::unique_ptr<Scene>>;

  template <typename T>
  using IsSceneType = typename std::enable_if_t<std::is_base_of_v<Scene, T>, bool>;

  class SceneManager {
  public:
    SceneManager(Application &app);

    void nextScene(bool rotate = false);
    void prevScene(bool rotate = false);
    void loadScene(size_t index);
    void loadScene(const std::string &name);
    void reloadCurrentScene();

    void update(float dt) const;
    bool hasScenes() const;

    /**
     * @brief Adds a new scene
     * @tparam T Type of system to register
     * @param name The name assigned to the new scene
     * @param Args Additional parameters required to construct the system
     */
    template <class T, typename... Args, IsSceneType<T> = true>
    void addScene(const std::string &name = "", Args &&...args) {
      size_t currentSceneIndex = hasScenes() ? 0 : m_sceneIt - m_scenes.begin();
      std::string sceneName = name.empty() ? std::format("Scene {0}", m_scenes.size() + 1) : name;
      m_scenes.emplace_back(std::make_unique<T>(sceneName, m_app, std::forward<Args>(args)...));
      m_sceneIt = m_scenes.begin() + currentSceneIndex; // reset iterator
    }

  private:
    void loadScene(SceneList::iterator it);

    Application &m_app;
    SceneList m_scenes;
    SceneList::iterator m_sceneIt;
  };
} // namespace TritiumEngine::Core
