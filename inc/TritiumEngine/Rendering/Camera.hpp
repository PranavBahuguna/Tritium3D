#pragma once

#include <TritiumEngine/Core/Transform.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  enum class ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

  class Camera {
  public:
    Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
           float farPlane, float fov = glm::radians(60.0f), const Transform &transform = Transform());

    /** Getter methods for camera properties */
    ProjectionType GetProjectionType() const;
    float GetFrameWidth() const;
    float GetFrameHeight() const;
    float GetAspectRatio() const;
    float GetNearPlane() const;
    float GetFarPlane() const;
    float GetFOV() const;
    glm::mat4 GetProjectionViewMatrix() const;

    /** Setter methods for camera properties */
    void SetProjectionType(ProjectionType projection);
    void SetFrameWidth(float frameWidth);
    void SetFrameHeight(float frameHeight);
    void SetNearPlane(float nearPlane);
    void SetFarPlane(float farPlane);
    void SetFOV(float fov);

  private:
    Transform m_transform;
    ProjectionType m_projection;
    float m_width;
    float m_height;
    float m_nearPlane;
    float m_farPlane;
    float m_fov;
  };
} // namespace TritiumEngine::Rendering