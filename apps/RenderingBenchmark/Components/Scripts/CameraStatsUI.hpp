#pragma once

#include <TritiumEngine/Core/Scriptable.hpp>

#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

#include <string>

using namespace TritiumEngine::Core;

namespace RenderingBenchmark::Scripts
{
  class CameraStatsUI : public Scriptable {
  public:
    CameraStatsUI(Application &app);

    void init() override{};
    void dispose() override{};
    void update(float dt) override;

  protected:
    void onEnable(bool enable) override;

  private:
    void initUI();
    void destroyUI();
    void addText(entt::entity &entity, const std::string &text, glm::vec3 position);

    entt::entity m_posXText  = entt::null;
    entt::entity m_posYText  = entt::null;
    entt::entity m_posZText  = entt::null;
    entt::entity m_pitchText = entt::null;
    entt::entity m_yawText   = entt::null;
    entt::entity m_fovText   = entt::null;

    inline static float m_sumDt = 0.f;
  };
} // namespace RenderingBenchmark::Scripts