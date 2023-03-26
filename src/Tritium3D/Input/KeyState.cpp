#include <Tritium3D/Input/KeyState.hpp>

#include <GLFW/glfw3.h>

namespace Tritium3D::Input
{
  void KeyState::Update(int action) {
    m_oldState = m_newState;
    m_newState = action;
  }

  bool KeyState::IsToggled() {
    bool isToggled = m_newState == GLFW_PRESS && m_oldState == GLFW_RELEASE;
    if (isToggled)
      Update(GLFW_RELEASE); // prevents any held presses from returning true

    return isToggled;
  }
} // namespace Tritium3D::Input