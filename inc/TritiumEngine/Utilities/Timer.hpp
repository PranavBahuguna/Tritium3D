#pragma once

namespace TritiumEngine::Utilities
{
  class Timer {
  public:
    static void Start();
    static void Update();
    static float DeltaTime();

  private:
    Timer() {} // prevent construction of this class
  };
} // namespace TritiumEngine::Utilities