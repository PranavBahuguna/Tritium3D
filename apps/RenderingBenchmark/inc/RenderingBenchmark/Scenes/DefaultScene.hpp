#pragma once

#include <TritiumEngine/Core/Scene.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;

namespace RenderingBenchmark::Scenes
{
  class DefaultScene : public Scene {
  public:
    DefaultScene();

  protected:
    void init() override;

  private:
    void setupCamera();
    void setupContainer();
    void createWall(float aX, float aY, float bX, float bY);
    void generateSquares(size_t n);
    void generateSquaresInstanced(size_t n);
  };
} // namespace RenderingBenchmark::Scenes