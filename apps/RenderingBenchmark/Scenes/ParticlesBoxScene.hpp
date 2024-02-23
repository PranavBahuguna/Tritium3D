#pragma once

#include <TritiumEngine/Core/Scene.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;

namespace RenderingBenchmark::Scenes
{
  class ParticlesBoxScene : public Scene {
  public:
    enum class RenderType { Default, Instanced, Geometry };

    ParticlesBoxScene();

  protected:
    void onRegister() override;
    void init() override;

  private:
    void setupSystems();
    void setupControls();
    void setupCamera();
    void setupContainer();

    void setRenderType(RenderType renderType);
    void setParticleCount(size_t nParticles);

    void createWall(float aX, float aY, float bX, float bY);
    void generateParticlesDefault();
    void generateParticlesInstanced();
    void generateParticlesGeometry();

    RenderType m_renderType;
    size_t m_nParticles;
  };
} // namespace RenderingBenchmark::Scenes