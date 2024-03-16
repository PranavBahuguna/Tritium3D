#pragma once

#include <entt/entity/entity.hpp>

#include <string>
#include <vector>

namespace TritiumEngine::Core
{
  class Application;
  class Scene;

  using SceneList = std::vector<std::unique_ptr<Scene>>;

  class SceneManager {
  public:
    SceneManager(Application *m_app);

    void addScene(std::unique_ptr<Scene> scene);
    void nextScene(bool rotate = false);
    void prevScene(bool rotate = false);
    void loadScene(size_t index);
    void loadScene(const std::string &name);
    void reloadCurrentScene();

    void update(float dt) const;
    bool hasScenes() const;

  private:
    void loadScene(SceneList::iterator it);

    Application *m_app;
    SceneList m_scenes;
    SceneList::iterator m_sceneIt;
  };
} // namespace TritiumEngine::Core
