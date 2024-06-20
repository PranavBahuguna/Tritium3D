#include <TritiumEngine/Utilities/CameraController.hpp>

using namespace TritiumEngine::Rendering;

namespace
{
  constexpr static float PI_RADIANS = glm::pi<float>();
} // namespace

using Projection = Camera::Projection;

namespace TritiumEngine::Utilities
{
  CameraController::CameraController(InputManager &inputManager)
      : m_input(inputManager), m_callbacks(), m_yaw(0.f), m_pitch(0.f) {}

  void CameraController::mapKey(Key key, CameraAction action) { m_actionKeys[action].insert(key); }

  void CameraController::init(Camera &camera, bool enableMouseMove, bool enableMouseScroll) {
    Transform &transform = camera.transform;

    for (auto key : m_actionKeys[CameraAction::MOVE_FORWARD])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += transform.getFront() * moveSpeed * getOrthoScale(camera).z * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::MOVE_BACKWARD])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += transform.getFront() * -moveSpeed * getOrthoScale(camera).z * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::MOVE_LEFT])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += transform.getRight() * -moveSpeed * getOrthoScale(camera).x * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::MOVE_RIGHT])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += transform.getRight() * moveSpeed * getOrthoScale(camera).x * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::MOVE_DOWN])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += Transform::UP * -moveSpeed * getOrthoScale(camera).y * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::MOVE_UP])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform, &camera](float dt) {
            transform.position += Transform::UP * moveSpeed * getOrthoScale(camera).y * dt;
          }));

    for (auto key : m_actionKeys[CameraAction::TURN_LEFT])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform](float dt) {
            addYaw(-turnSpeed * dt);
            transform.setRotation(m_pitch, m_yaw, 0.f);
          }));

    for (auto key : m_actionKeys[CameraAction::TURN_RIGHT])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform](float dt) {
            addYaw(turnSpeed * dt);
            transform.setRotation(m_pitch, m_yaw, 0.f);
          }));

    for (auto key : m_actionKeys[CameraAction::TURN_DOWN])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform](float dt) {
            addPitch(-turnSpeed * dt);
            transform.setRotation(m_pitch, m_yaw, 0.f);
          }));

    for (auto key : m_actionKeys[CameraAction::TURN_UP])
      m_callbacks.push_back(
          m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform](float dt) {
            addPitch(turnSpeed * dt);
            transform.setRotation(m_pitch, m_yaw, 0.f);
          }));

    for (auto key : m_actionKeys[CameraAction::ZOOM_IN])
      m_callbacks.push_back(m_input.addKeyCallback(
          key, KeyState::PRESSED, [this, &camera](float dt) { addZoom(camera, -zoomSpeed * dt); }));

    for (auto key : m_actionKeys[CameraAction::ZOOM_OUT])
      m_callbacks.push_back(m_input.addKeyCallback(
          key, KeyState::PRESSED, [this, &camera](float dt) { addZoom(camera, zoomSpeed * dt); }));

    for (auto key : m_actionKeys[CameraAction::CENTER])
      m_callbacks.push_back(m_input.addKeyCallback(key, KeyState::PRESSED, [this, &transform]() {
        m_pitch = 0.f;
        transform.setRotation(m_pitch, m_yaw, 0.f);
      }));

    if (enableMouseMove)
      m_callbacks.push_back(
          m_input.addMouseMoveCallback([this, &transform](float dt, double deltaX, double deltaY) {
            addPitch(-static_cast<float>(deltaY) * turnSpeed * mouseTurnSensitivity * dt);
            addYaw(static_cast<float>(deltaX) * turnSpeed * mouseTurnSensitivity * dt);
            transform.setRotation(m_pitch, m_yaw, 0.f);
          }));

    if (enableMouseScroll)
      m_callbacks.push_back(
          m_input.addMouseScrollCallback([this, &camera](float dt, double xOffset, double yOffset) {
            addZoom(camera, static_cast<float>(yOffset) * zoomSpeed * mouseZoomSensitivity * dt);
          }));
  }

  void CameraController::dispose() {
    m_input.removeCallbacks(m_callbacks);
    m_callbacks.clear();
    m_pitch = 0.f;
    m_yaw   = 0.f;
  }

  void CameraController::addPitch(float pitch) {
    m_pitch += pitch;
    m_pitch = std::min(m_pitch, maxPitch);
    m_pitch = std::max(m_pitch, minPitch);
  }

  void CameraController::addYaw(float yaw) {
    m_yaw += yaw;
    if (m_yaw > PI_RADIANS || m_yaw < -PI_RADIANS) {
      float adjust = m_yaw >= 0.f ? PI_RADIANS : -PI_RADIANS;
      m_yaw        = fmodf(m_yaw, PI_RADIANS) - adjust;
    }
  }

  void CameraController::addZoom(Camera &camera, float zoom) {
    if (camera.projection == Camera::Projection::ORTHOGRAPHIC) {
      camera.transform.scale += zoom;
      camera.transform.scale.x = std::min(camera.transform.scale.x, maxOrthographicZoom);
      camera.transform.scale.y = std::min(camera.transform.scale.y, maxOrthographicZoom);
      camera.transform.scale.x = std::max(camera.transform.scale.x, minOrthographicZoom);
      camera.transform.scale.y = std::max(camera.transform.scale.y, minOrthographicZoom);
    } else {
      camera.fov += zoom;
      camera.fov = std::min(camera.fov, maxFov);
      camera.fov = std::max(camera.fov, minFov);
    }
  }

  glm::vec3 CameraController::getOrthoScale(const Camera &camera) const {
    bool useOrthoScale =
        camera.projection == Projection::ORTHOGRAPHIC && scaleOrthographicMoveSpeedWithZoom;
    return useOrthoScale ? camera.transform.scale : glm::vec3{1.f};
  }
} // namespace TritiumEngine::Utilities