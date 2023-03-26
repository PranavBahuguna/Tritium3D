#pragma once

namespace Tritium3D::Input
{
  struct KeyState {
  public:
    void Update(int action);
    bool IsToggled();

  private:
    int m_oldState;
    int m_newState;
  };
} // namespace Tritium3D::Input