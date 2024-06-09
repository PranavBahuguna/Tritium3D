#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Rendering/TextRendering/Components/Text.hpp>

#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Core
{
  class Application;
}

namespace RenderingBenchmark::Scenes
{
  using Application = TritiumEngine::Core::Application;

  class ParticlesBoxScene : public Scene {
  public:
    enum class RenderType { Default, Instanced, Geometry };

    ParticlesBoxScene(const std::string &name, Application &app);

  protected:
    void init() override;
    void dispose() override;

  private:
    void setupContainer();
    void setupParticles();

    void setRenderType(RenderType renderType);
    void setParticleCount(int nParticles);

    entt::entity addText(const std::string &text, const glm::vec2 &position, float scaleFactor,
                         Text::Alignment alignment);
    void createWall(float aX, float aY, float bX, float bY);
    void generateParticlesDefault();
    void generateParticlesInstanced();
    void generateParticlesGeometry();

    RenderType m_renderType;
    int m_nParticles;
    CallbackId m_callbacks[11];
    entt::entity m_titleText = entt::null;
  };
} // namespace RenderingBenchmark::Scenes