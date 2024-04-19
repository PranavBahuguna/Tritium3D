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
    auto it = std::next(m_colorPoints.begin(), index);
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
    if (lessThanAll || greaterThanAll) {
      x = last;
      y = first;
    } else {
      // Iterate over the color points to find the interval the value falls into
      for (auto it = std::next(m_colorPoints.begin()); it != m_colorPoints.end(); ++it) {
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
    auto xColor = ColorUtils::ToVec4(x.color);
    auto yColor = ColorUtils::ToVec4(y.color);

    // Calculate a normalized value for the interval
    float diff            = y.value - x.value;
    float intervalWidth   = diff <= 0.f ? diff + 1.f : diff;
    float normalizedValue = fmodf(value - x.value, 1.f) / intervalWidth;

    uint8_t r = (uint8_t)((1.f - normalizedValue) * xColor.r + normalizedValue * yColor.r);
    uint8_t g = (uint8_t)((1.f - normalizedValue) * xColor.g + normalizedValue * yColor.g);
    uint8_t b = (uint8_t)((1.f - normalizedValue) * xColor.b + normalizedValue * yColor.b);
    uint8_t a = (uint8_t)((1.f - normalizedValue) * xColor.a + normalizedValue * yColor.a);

    return ColorUtils::FromRGBAComponents(r, g, b, a);
  }
} // namespace TritiumEngine::Rendering