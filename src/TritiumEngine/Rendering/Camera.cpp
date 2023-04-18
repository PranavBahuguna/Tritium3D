#include <TritiumEngine/Rendering/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Rendering
{
  Camera::Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
                 float farPlane, float fov, const Transform &transform)
      : m_transform(transform), m_projection(projection), m_width(frameWidth),
        m_height(frameHeight), m_nearPlane(nearPlane), m_farPlane(farPlane), m_fov(fov) {}

  ProjectionType Camera::GetProjectionType() const { return m_projection; }

  float Camera::GetFrameWidth() const { return m_width; }

  float Camera::GetFrameHeight() const { return m_height; }

  float Camera::GetAspectRatio() const { return m_width / m_height; }

  float Camera::GetNearPlane() const { return m_nearPlane; }

  float Camera::GetFarPlane() const { return m_farPlane; }

  float Camera::GetFOV() const { return m_fov; }

  glm::mat4 Camera::GetProjectionViewMatrix() const {
    glm::mat4 projection = glm::mat4();

    switch (m_projection) {
    case ProjectionType::ORTHOGRAPHIC:
      projection = glm::ortho(-m_width * 0.5f, m_width * 0.5f, -m_height * 0.5f, m_height * 0.5f);
      break;
    case ProjectionType::PERSPECTIVE:
      projection = glm::perspective(m_fov, GetAspectRatio(), m_nearPlane, m_farPlane);
      break;
    }

    glm::mat4 projectionView = projection * m_transform.GetViewMatrix();

    return projectionView;
  }

  void Camera::SetProjectionType(ProjectionType projection) { m_projection = projection; }

  void Camera::SetFrameWidth(float frameWidth) { m_width = frameWidth; }

  void Camera::SetFrameHeight(float frameHeight) { m_height = frameHeight; }

  void Camera::SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

  void Camera::SetFarPlane(float farPlane) { m_farPlane = farPlane; }

  void Camera::SetFOV(float fov) { m_fov = fov; }
} // namespace TritiumEngine::Rendering