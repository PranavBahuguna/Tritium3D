#include <TritiumEngine/Utilities/Timer.hpp>

#include <chrono>
#include <stdexcept>

namespace TritiumEngine::Utilities
{
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::steady_clock::time_point TimePoint;

  static TimePoint prevTime;
  static TimePoint currentTime;
  static float deltaTime;
  static bool isInitialised = false;

  void Timer::Start() {
    isInitialised = true;
    prevTime = Clock::now();
    currentTime = prevTime;
    Update();
  }

  void Timer::Update() {
    if (!isInitialised)
      throw std::runtime_error("Error, timer must be initialised first before update!");

    prevTime = currentTime;
    currentTime = Clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - prevTime).count();
  }

  float Timer::DeltaTime() { return deltaTime; }
} // namespace TritiumEngine::Utilities