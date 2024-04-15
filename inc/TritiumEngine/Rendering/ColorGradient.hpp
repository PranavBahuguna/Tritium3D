#pragma once

#include <TritiumEngine/Rendering/Components/Color.hpp>

#include <list>

namespace TritiumEngine::Rendering
{
  using ColorPoint = std::pair<Color, float>;

  class ColorGradient {
  public:
    struct ColorPoint {
      Color color;
      float value;
    };

    ColorGradient(const std::list<ColorPoint> &colorPoints = {}, bool loop = false);

    void addColorPoint(Color color, float value);
    void addColorPoint(ColorPoint colorPoint);
    void removeAtIndex(size_t index);
    void setGradientLoopEnabled(bool enabled);

    Color getColor(float value) const;

  private:
    std::list<ColorPoint> m_colorPoints;
    bool m_loop;
  };
} // namespace TritiumEngine::Rendering