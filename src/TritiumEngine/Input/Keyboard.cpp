#include <TritiumEngine/Input/Keyboard.hpp>

#include <string>

namespace TritiumEngine::Input
{
  KeyState Keyboard::_keys[NUM_KEYS] = {};

  void Keyboard::KeyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (ValidateKey(key))
      _keys[key].Update(action);
  }

  bool Keyboard::GetKeyToggled(int key) {
    if (!ValidateKey(key))
      return false;

    return _keys[key].IsToggled();
  }

  bool Keyboard::ValidateKey(int key) {
    bool isKeyValid = key >= 0 || key <= NUM_KEYS;
    if (!isKeyValid)
      printf("Key with code %d is outside of valid range!\n", key);

    return isKeyValid;
  }
} // namespace TritiumEngine::Input