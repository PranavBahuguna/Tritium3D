#include <TritiumEngine/Rendering/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Rendering
{
  Camera::Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
                 float farPlane, float fov, const Transform &transform)
      : m_transform(transform), m_projection(projection), m_width(frameWidth),
        m_height(frameHeight), m_nearPlane(nearPlane), m_farPlane(farPlane), m_fov(fov) {}

  ProjectionType Camera::getProjectionType() const { return m_projection; }

  float Camera::getFrameWidth() const { return m_width; }

  float Camera::getFrameHeight() const { return m_height; }

  float Camera::getAspectRatio() const { return m_width / m_height; }

  float Camera::getNearPlane() const { return m_nearPlane; }

  float Camera::getFarPlane() const { return m_farPlane; }

  float Camera::getFOV() const { return m_fov; }

  glm::mat4 Camera::getProjectionViewMatrix() const {
    glm::mat4 projection = glm::mat4();

    switch (m_projection) {
    case ProjectionType::ORTHOGRAPHIC:
      projection = glm::ortho(-m_width * 0.5f, m_width * 0.5f, -m_height * 0.5f, m_height * 0.5f);
      break;
    case ProjectionType::PERSPECTIVE:
      projection = glm::perspective(m_fov, getAspectRatio(), m_nearPlane, m_farPlane);
      break;
    }

    glm::mat4 projectionView = projection * m_transform.getViewMatrix();

    return projectionView;
  }

  void Camera::setProjectionType(ProjectionType projection) { m_projection = projection; }

  void Camera::setFrameWidth(float frameWidth) { m_width = frameWidth; }

  void Camera::setFrameHeight(float frameHeight) { m_height = frameHeight; }

  void Camera::setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

  void Camera::setFarPlane(float farPlane) { m_farPlane = farPlane; }

  void Camera::setFOV(float fov) { m_fov = fov; }
} // namespace TritiumEngine::Rendering