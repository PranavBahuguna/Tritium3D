#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/TextRendering/Components/Text.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace RenderingBenchmark::Scenes
{
  class ParticlesBoxScene : public Scene {
  public:
    enum class RenderType { Default, Instanced, Geometry };

    ParticlesBoxScene();

  protected:
    void init() override;
    void dispose() override;

  private:
    void setupSystems();
    void setupControls();
    void setupCamera();
    void setupContainer();
    void initUI();
    void setupParticles();
    entt::entity addText(const std::string &text, const glm::vec2 &position, float scale,
                         Text::Alignment alignment);

    void setRenderType(RenderType renderType);
    void setParticleCount(int nParticles);

    void createWall(float aX, float aY, float bX, float bY);
    void generateParticlesDefault();
    void generateParticlesInstanced();
    void generateParticlesGeometry();

    RenderType m_renderType;
    int m_nParticles;
    CallbackId m_callbacks[11];
    entt::entity m_titleText  = entt::null;
    entt::entity m_fpsStatsUI = entt::null;
  };
} // namespace RenderingBenchmark::Scenes