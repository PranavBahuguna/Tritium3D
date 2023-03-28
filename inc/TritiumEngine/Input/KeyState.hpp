#pragma once

namespace TritiumEngine::Input
{
  struct KeyState {
  public:
    void Update(int action);
    bool IsToggled();

  private:
    int m_oldState;
    int m_newState;
  };
} // namespace TritiumEngine::Input