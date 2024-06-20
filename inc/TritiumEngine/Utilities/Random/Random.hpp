#pragma once

#include <random>

namespace TritiumEngine::Utilities::Random::Internal
{
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<float> uniformDist(0.f, 1.f);
} // namespace TritiumEngine::Utilities::Random