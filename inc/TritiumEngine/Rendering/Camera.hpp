#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <TritiumEngine/Core/Transform.hpp>

#include <glm/glm.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  enum class ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

  class Camera {
  public:
    Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
           float farPlane, float fov = glm::radians(60.0f),
           const Transform &transform = Transform());

    // Getter methods for camera properties
    ProjectionType getProjectionType() const;
    float getFrameWidth() const;
    float getFrameHeight() const;
    float getAspectRatio() const;
    float getNearPlane() const;
    float getFarPlane() const;
    float getFOV() const;
    glm::mat4 getProjectionViewMatrix() const;

    // Setter methods for camera properties
    void setProjectionType(ProjectionType projection);
    void setFrameWidth(float frameWidth);
    void setFrameHeight(float frameHeight);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);
    void setFOV(float fov);

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

#endif // CAMERA_HPP