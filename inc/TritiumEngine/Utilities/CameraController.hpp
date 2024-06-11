#pragma once

#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Rendering/Components/Camera.hpp>

#include <unordered_set>

using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Utilities
{
  enum class CameraAction : uint8_t {
    MOVE_FORWARD = 0,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_DOWN,
    TURN_UP,
    ZOOM_IN,
    ZOOM_OUT,
    CENTER
  };

  class CameraController {
  public:
    CameraController(InputManager &inputManager);

    void init(Camera &camera, bool enableMouseMove = true, bool enableMouseScroll = true);
    void dispose();
    void mapKey(Key key, CameraAction action);

    float getPitch() const { return m_pitch; }
    float getYaw() const { return m_yaw; }

    float moveSpeed                         = 12.0f;
    float turnSpeed                         = 1.0f;
    float zoomSpeed                         = 0.5f;
    float mouseTurnSensitivity              = 0.33f;
    float mouseZoomSensitivity              = 1.f;
    float minPitch                          = glm::radians(-89.f);
    float maxPitch                          = glm::radians(89.f);
    float minFov                            = glm::radians(20.f);
    float maxFov                            = glm::radians(90.f);
    float minOrthographicZoom               = 0.1f;
    float maxOrthographicZoom               = 10.f;
    bool scaleOrthographicMoveSpeedWithZoom = true;

  private:
    void addPitch(float pitch);
    void addYaw(float yaw);
    void addZoom(Camera &camera, float zoom);

    glm::vec3 getOrthoScale(const Camera &camera) const;

    InputManager &m_input;
    std::unordered_map<CameraAction, std::unordered_set<Key>> m_actionKeys;
    std::vector<CallbackId> m_callbacks;
    float m_yaw;
    float m_pitch;
  };
} // namespace TritiumEngine::Utilities