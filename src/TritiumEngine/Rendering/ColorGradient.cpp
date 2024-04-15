#include <TritiumEngine/Rendering/ColorGradient.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

#include <algorithm>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  ColorGradient::ColorGradient(const std::list<ColorPoint> &colorPoints, bool loop)
      : m_colorPoints(colorPoints), m_loop(loop) {
    m_colorPoints.sort([](const auto &a, const auto &b) { return a.value > b.value; });
    for (ColorPoint &colorPoint : m_colorPoints)
      colorPoint.value = std::clamp(colorPoint.value, 0.f, 1.f);
  }

  /**
   * @brief Adds a new color point
   */
  void ColorGradient::addColorPoint(Color color, float value) { addColorPoint({color, value}); }

  void ColorGradient::addColorPoint(ColorPoint colorPoint) {
    colorPoint.value = std::clamp(colorPoint.value, 0.f, 1.f);
    m_colorPoints.insert(std::lower_bound(m_colorPoints.begin(), m_colorPoints.end(), colorPoint,
                                          [](const ColorPoint &c1, const ColorPoint &c2) {
                                            return c1.value < c2.value;
                                          }),
                         colorPoint);
  }

  void ColorGradient::removeAtIndex(size_t index) {
    auto it = m_colorPoints.begin();
    std::advance(it, index);
    m_colorPoints.erase(it);
  }

  void ColorGradient::setGradientLoopEnabled(bool enabled) { m_loop = enabled; }

  Color ColorGradient::getColor(float value) const {
    if (m_colorPoints.empty())
      return COLOR_NONE; // default - no color
    if (m_colorPoints.size() == 1)
      return m_colorPoints.front().color; // single color gradient

    // Clamp value between 0 and 1
    value = std::max(value, 0.f);
    value = std::min(value, 1.f);

    ColorPoint first = m_colorPoints.front();
    ColorPoint last  = m_colorPoints.back();

    // Check for values less than or greater than all color points. If not looping, calculated color
    // will be the first or last color point respectively
    bool lessThanAll    = value < first.value;
    bool greaterThanAll = value >= last.value;
    if (!m_loop) {
      if (lessThanAll)
        return first.color;
      if (greaterThanAll)
        return last.color;
    }

    ColorPoint x;
    ColorPoint y;

    // If looping, interval will be calculated between last color point and first
    bool loopingInterval = false;
    if (lessThanAll || greaterThanAll) {
      x               = last;
      y               = first;
      loopingInterval = true;
    } else {
      // Iterate over the color points to find the interval the value falls into
      auto it = m_colorPoints.begin();
      ++it;

      for (; it != m_colorPoints.end(); ++it) {
        if (it->value == value)
          return it->color; // if value matches exactly, return its color

        // Interval consists of the prior color point, and the first point with a value greater than
        // the provided value
        if (it->value > value) {
          y = *it;
          x = *(--it);
          break;
        }
      }
    }

    // Obtain colors as a vector of 8 bit RGBA components
    auto aColor = ColorUtils::ToVec4(x.color);
    auto bColor = ColorUtils::ToVec4(y.color);

    // Calculate a normalized value for the interval
    float normalizedValue;
    float intervalWidth;

    if (!loopingInterval) {
      intervalWidth   = y.value - x.value;
      normalizedValue = (value - x.value) / intervalWidth;
    } else {
      intervalWidth   = y.value - x.value + 1.f;
      normalizedValue = (value > 0.5f) ? (value - x.value) / intervalWidth
                                       : ((value - y.value) / intervalWidth) + 1.f;
    }

    uint8_t r = (uint8_t)((1.f - normalizedValue) * aColor.r + normalizedValue * bColor.r);
    uint8_t g = (uint8_t)((1.f - normalizedValue) * aColor.g + normalizedValue * bColor.g);
    uint8_t b = (uint8_t)((1.f - normalizedValue) * aColor.b + normalizedValue * bColor.b);
    uint8_t a = (uint8_t)((1.f - normalizedValue) * aColor.a + normalizedValue * bColor.a);

    return ColorUtils::FromRGBAComponents(r, g, b, a);
  }
} // namespace TritiumEngine::Rendering