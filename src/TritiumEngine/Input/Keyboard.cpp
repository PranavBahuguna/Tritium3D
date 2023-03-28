#include <TritiumEngine/Input/Keyboard.hpp>

#include <string>

namespace TritiumEngine::Input
{
  Keyboard::KeyState Keyboard::_keys[NUM_KEYS] = {};

  /**
   * Implementation of GLFWkeyfun function pointer type for handling keyboard input.
   *
   * @param window The window receiving the key event
   * @param key The key pressed / released.
   * @param scancode [UNUSED] Platform-specific scancode of the key.
   * @param action The code of the action performed on the key.
   * @param mods [UNUSED] Bit-field describing which modifier keys were also held down.
   */
  void Keyboard::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (ValidateKey(key))
      _keys[key].Update(action);
  }

  /**
   * Determines if the given key has been toggled.
   *
   * @param key The supplied key.
   * @return True if key has been toggled.
   */
  bool Keyboard::GetKeyToggled(int key) {
    if (!ValidateKey(key))
      return false;

    return _keys[key].IsToggled();
  }

  // Check if keycode falls within valid range
  bool Keyboard::ValidateKey(int key) {
    bool isKeyValid = key >= 0 || key <= NUM_KEYS;
    if (!isKeyValid)
      printf("Key with code %d is outside of valid range!\n", key);

    return isKeyValid;
  }

  // Updates key state according to action
  void Keyboard::KeyState::Update(int action) {
    m_oldState = m_newState;
    m_newState = action;
  }

  bool Keyboard::KeyState::IsToggled() {
    bool isToggled = m_newState == GLFW_PRESS && m_oldState == GLFW_RELEASE;
    if (isToggled)
      Update(GLFW_RELEASE); // prevents any held presses from returning true

    return isToggled;
  }
} // namespace TritiumEngine::Input