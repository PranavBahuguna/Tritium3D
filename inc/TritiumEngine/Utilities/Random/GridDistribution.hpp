#pragma once

#include <TritiumEngine/Utilities/Logger.hpp>
#include <TritiumEngine/Utilities/Random/Random.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Utilities;
using namespace TritiumEngine::Utilities::Random::Internal;

namespace TritiumEngine::Utilities::Random
{
  class GridDistribution {
  public:
    GridDistribution(size_t cols, size_t rows, float cellWidth, float cellHeight,
                     const float displacement = 1.f)
        : m_cols(cols), m_rows(rows), m_cellIndex(0), m_cellWidth(cellWidth),
          m_cellHeight(cellHeight), m_displacement(displacement) {
      // Validate grid size
      if (m_cols > SIZE_MAX / m_rows)
        throw std::invalid_argument(
            std::format("Grid with {} columns {} rows exceeds numeric limits!", m_cols, m_rows));

      m_nCells = m_cols * m_rows;
    }

    size_t getNumCells() const { return m_nCells; }

    bool hasNext() const { return m_cellIndex < m_nCells; }

    glm::vec3 getNext() {
      float col          = static_cast<float>(m_cellIndex % m_cols);
      float row          = static_cast<float>(m_cellIndex / m_cols);
      float startX       = 0.5f * (1.f - (float)m_cols);
      float startY       = 0.5f * (1.f - (float)m_rows);
      float displacement = (uniformDist(mt) - 0.5f) * m_displacement;
      float cellX        = (startX + col + displacement) * m_cellWidth;
      float cellY        = (startY + row + displacement) * m_cellHeight;
      ++m_cellIndex;

      return {cellX, cellY, 0.f};
    }

  private:
    size_t m_cols;
    size_t m_rows;
    float m_cellWidth;
    float m_cellHeight;
    float m_displacement;
    size_t m_nCells;
    size_t m_cellIndex;
  };
} // namespace TritiumEngine::Utilities::Random
