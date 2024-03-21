#include <TritiumEngine/Utilities/CameraController.hpp>

using namespace TritiumEngine::Rendering;

namespace
{
  constexpr static float MOVE_SPEED = 5.0f;
  constexpr static float TURN_SPEED = 1.0f;
  constexpr static float ZOOM_SPEED = 0.5f;
  constexpr static float MIN_PITCH  = glm::radians(-89.f);
  constexpr static float MAX_PITCH  = glm::radians(89.f);
  constexpr static float MIN_FOV    = glm::radians(20.f);
  constexpr static float MAX_FOV    = glm::radians(90.f);
  constexpr static float PI_RADIANS = glm::pi<float>();
} // namespace

namespace TritiumEngine::Utilities
{
  CameraController::CameraController() : m_callbacks(), m_yaw(0.f), m_pitch(0.f) {}

  void CameraController::registerWindow(Window &window) { m_window = &window; }

  void CameraController::mapKey(Key key, CameraAction action) { m_actionKey[action] = key; }

  void CameraController::init(Camera &camera) {
    Transform &transform = camera.transform;

    m_callbacks[0] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_FORWARD], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getFront() * MOVE_SPEED * dt); });
    m_callbacks[1] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_BACKWARD], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getFront() * -MOVE_SPEED * dt); });
    m_callbacks[2] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_LEFT], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getRight() * -MOVE_SPEED * dt); });
    m_callbacks[3] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_RIGHT], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getRight() * MOVE_SPEED * dt); });
    m_callbacks[4] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_UP], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getUp() * MOVE_SPEED * dt); });
    m_callbacks[5] = m_window->addKeyCallback(
        m_actionKey[CameraAction::MOVE_DOWN], KeyState::PRESSED,
        [&transform](float dt) { transform.translate(transform.getUp() * -MOVE_SPEED * dt); });
    m_callbacks[6] = m_window->addKeyCallback(m_actionKey[CameraAction::TURN_LEFT],
                                              KeyState::PRESSED, [this, &transform](float dt) {
                                                addYaw(-TURN_SPEED * dt);
                                                transform.setRotation(m_pitch, m_yaw);
                                              });
    m_callbacks[7] = m_window->addKeyCallback(m_actionKey[CameraAction::TURN_RIGHT],
                                              KeyState::PRESSED, [this, &transform](float dt) {
                                                addYaw(TURN_SPEED * dt);
                                                transform.setRotation(m_pitch, m_yaw);
                                              });
    m_callbacks[8] = m_window->addKeyCallback(m_actionKey[CameraAction::TURN_UP], KeyState::PRESSED,
                                              [this, &transform](float dt) {
                                                addPitch(-TURN_SPEED * dt);
                                                transform.setRotation(m_pitch, m_yaw);
                                              });
    m_callbacks[9] = m_window->addKeyCallback(m_actionKey[CameraAction::TURN_DOWN],
                                              KeyState::PRESSED, [this, &transform](float dt) {
                                                addPitch(TURN_SPEED * dt);
                                                transform.setRotation(m_pitch, m_yaw);
                                              });
    m_callbacks[10] =
        m_window->addMouseMoveCallback([this, &transform](float dt, double deltaX, double deltaY) {
          addPitch(static_cast<float>(deltaY) * TURN_SPEED * dt);
          addYaw(static_cast<float>(deltaX) * TURN_SPEED * dt);
          transform.setRotation(m_pitch, m_yaw);
        });
    m_callbacks[11] =
        m_window->addMouseScrollCallback([this, &camera](float dt, double xOffset, double yOffset) {
          addZoom(camera, static_cast<float>(yOffset) * ZOOM_SPEED * dt);
        });
    m_callbacks[12] =
        m_window->addKeyCallback(m_actionKey[CameraAction::ZOOM_IN], KeyState::PRESSED,
                                 [this, &camera](float dt) { addZoom(camera, -ZOOM_SPEED * dt); });
    m_callbacks[13] =
        m_window->addKeyCallback(m_actionKey[CameraAction::ZOOM_OUT], KeyState::PRESSED,
                                 [this, &camera](float dt) { addZoom(camera, ZOOM_SPEED * dt); });
    m_callbacks[14] = m_window->addKeyCallback(m_actionKey[CameraAction::CENTER],
                                               KeyState::START_PRESS, [this, &transform]() {
                                                 m_pitch = 0.f;
                                                 transform.setRotation(m_pitch, m_yaw);
                                               });
  }

  void CameraController::dispose() {
    m_window->removeCallbacks(m_callbacks);
    m_pitch = 0.f;
    m_yaw   = 0.f;
  }

  void CameraController::addPitch(float pitch) {
    m_pitch += pitch;
    m_pitch = std::min(m_pitch, MAX_PITCH);
    m_pitch = std::max(m_pitch, MIN_PITCH);
  }

  void CameraController::addYaw(float yaw) {
    m_yaw += yaw;
    if (m_yaw > PI_RADIANS || m_yaw < -PI_RADIANS) {
      float adjust = m_yaw >= 0.f ? PI_RADIANS : -PI_RADIANS;
      m_yaw        = fmodf(m_yaw, PI_RADIANS) - adjust;
    }
  }

  void CameraController::addZoom(Camera &camera, float zoom) {
    camera.fov += zoom;
    camera.fov = std::min(camera.fov, MAX_FOV);
    camera.fov = std::max(camera.fov, MIN_FOV);
  }
} // namespace TritiumEngine::Utilities