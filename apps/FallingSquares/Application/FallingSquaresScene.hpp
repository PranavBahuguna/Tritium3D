#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/RenderData.hpp>

#include <glm/glm.hpp>

#include <random>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

class FallingSquaresScene : public Scene {
public:
  FallingSquaresScene();

protected:
  void init() override;

private:
  void setupCamera();
  void generateSquares(size_t n);
  void generateSquaresInstanced(size_t n);

  const RenderData m_squareRenderData{2,
                                      {
                                          0.5f, 0.5f,   // top right
                                          0.5f, -0.5f,  // bottom right
                                          -0.5f, -0.5f, // bottom left
                                          -0.5f, 0.5f   // top left
                                      },
                                      {
                                          0, 1, 2, // first triangle
                                          0, 2, 3  // second triangle
                                      }};

  const glm::vec3 m_startPos{0.f, 100.f, 0.f};

  std::random_device m_rd;
  std::mt19937 m_mt;
  std::uniform_real_distribution<float> m_rand;
  std::uniform_int_distribution<uint32_t> m_rand256;
};