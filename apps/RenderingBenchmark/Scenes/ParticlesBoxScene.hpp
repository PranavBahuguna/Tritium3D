#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering::TextRendering;

namespace RenderingBenchmark::Scenes
{
  class ParticlesBoxScene : public Scene {
  public:
    enum class RenderType { Default, Instanced, Geometry };

    ParticlesBoxScene();

  protected:
    void onRegister() override;
    void onUpdate(float dt) override;
    void init() override;

  private:
    void setupSystems();
    void setupControls();
    void setupCamera();
    void setupContainer();
    void setupUI();
    entt::entity addText(const std::string &text, const glm::vec2 &position, float scale,
                         Text::Alignment alignment);
    void setText(entt::entity textEntity, const std::string &textString);

    void setRenderType(RenderType renderType);
    void setParticleCount(int nParticles);

    void createWall(float aX, float aY, float bX, float bY);
    void generateParticlesDefault();
    void generateParticlesInstanced();
    void generateParticlesGeometry();

    RenderType m_renderType;
    int m_nParticles;

    // Dynamic UI entities
    entt::entity m_titleText{};
    entt::entity m_fpsText{};
    entt::entity m_frameTimeText{};
  };
} // namespace RenderingBenchmark::Scenes