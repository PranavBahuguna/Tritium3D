#pragma once

#include <TritiumEngine/Core/Scriptable.hpp>

#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

#include <string>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Utilities
{
  class FpsStatsUI : public Scriptable {
  public:
    FpsStatsUI(Application &app);

    void init() override{};
    void dispose() override{};
    void update(float dt) override;

  protected:
    void onEnable(bool enable) override;

  private:
    void initUI();
    void destroyUI();
    void addText(entt::entity &entity, const std::string &text, glm::vec3 position);

    entt::entity m_fpsText       = entt::null;
    entt::entity m_frameTimeText = entt::null;

    inline static int m_nFrames = 0;
    inline static float m_sumDt = 0.f;
  };
} // namespace TritiumEngine::Utilities