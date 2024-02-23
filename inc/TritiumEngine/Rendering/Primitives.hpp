#pragma once

#include <TritiumEngine/Rendering/RenderData.hpp>

#include <glm/glm.hpp>

#include <numbers>

namespace TritiumEngine::Rendering::Primitives
{
  /**
   * @brief Point generator helper method, vertices only
   */
  static RenderData createPoint(float x, float y) {
    std::vector<float> vertices {x, y};
    return RenderData{2, vertices};
  }

  /**
   * @brief Line generator helper method, vertices only
   */
  static RenderData createLine(float aX, float aY, float bX, float bY) {
    std::vector<float> vertices{
        aX, aY, // line start
        bX, bY  // line end
    };

    return RenderData{2, vertices};
  }

  /**
   * @brief Triangle generator helper method, vertices only
   */
  static RenderData createTriangle() {
    std::vector<float> vertices{
        -0.5f, -0.5f, // bottom left
        0.5f,  -0.5f, // bottom right
        0.0f,  0.5f   // top middle
    };

    return RenderData{2, vertices};
  }

  /**
   * @brief Square generator helper method, uses vertices and indices
   */
  static RenderData createSquare() {
    std::vector<float> vertices{
        0.5f,  0.5f,  // top right
        0.5f,  -0.5f, // bottom right
        -0.5f, -0.5f, // bottom left
        -0.5f, 0.5f   // top left
    };

    std::vector<unsigned int> indices{
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    return RenderData{2, vertices, indices};
  }

  /**
   * @brief Circle generator helper method, vertices only. Circles created with a center vertex, so
   * can be filled when rendering with GL_TRINAGLE_FAN.
   * @param nEdges Number of edges in the composed circle. Higher values produce a smoother circle.
   */
  static RenderData createCircle(size_t nEdges = 100) {
    std::vector<float> vertices;
    vertices.resize(2 * (nEdges + 2));

    // Add the center vertex (if not hollow)
    vertices[0] = 0.f;
    vertices[1] = 0.f;

    // Add all the circle vertices
    float twoPi  = 2.f * static_cast<float>(std::numbers::pi);
    float radius = 0.5f;

    for (size_t i = 0; i < nEdges + 1; ++i) {
      size_t index        = 2 * (i + 1);
      vertices[index]     = radius * cos(i * twoPi / nEdges);
      vertices[index + 1] = radius * sin(i * twoPi / nEdges);
    }

    return RenderData{2, vertices};
  }

  /**
   * @brief Circle generator helper method, vertices only. Circles created without a center vertex,
   * so should rendered with GL_LINE_LOOP.
   * @param nEdges Number of edges in the composed circle. Higher values produce a smoother circle.
   */
  static RenderData createHollowCircle(size_t nEdges = 100) {
    std::vector<float> vertices;
    vertices.resize(2 * nEdges);

    // Add all the circle vertices
    float twoPi  = 2.f * static_cast<float>(std::numbers::pi);
    float radius = 0.5f;

    for (size_t i = 0; i < nEdges; ++i) {
      size_t index        = 2 * i;
      vertices[index]     = radius * cos(i * twoPi / nEdges);
      vertices[index + 1] = radius * sin(i * twoPi / nEdges);
    }

    return RenderData{2, vertices};
  }
} // namespace TritiumEngine::Rendering::Primitives