#ifndef TIMER_HPP
#define TIMER_HPP

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

#endif TIMER_HPP