#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/RenderData.hpp>

#include <glm/glm.hpp>

#include <random>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

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

  glm::vec3 randRadialPosition(float radius, bool uniform);

  std::random_device m_rd;
  std::mt19937 m_mt;
  std::uniform_real_distribution<float> m_rand;
};