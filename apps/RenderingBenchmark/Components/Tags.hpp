#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/core/type_traits.hpp>

using namespace entt::literals;

namespace RenderingBenchmark::Components
{
  using MainCameraTag = entt::tag<"mainCamera"_hs>;
  using UiCameraTag   = entt::tag<"uiCamera"_hs>;
} // namespace RenderingBenchmark::Components