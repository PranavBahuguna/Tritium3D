#include <TritiumEngine/Core/Components/Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Core
{
  Transform::Transform(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale)
      : m_position(position), m_rotation(rotation), m_scale(scale) {}

  const glm::vec3 &Transform::getPosition() const { return m_position; }

  const glm::quat &Transform::getRotation() const { return m_rotation; }

  const glm::vec3 &Transform::getScale() const { return m_scale; }

  glm::vec3 Transform::getRight() const {
    return glm::rotate(glm::conjugate(m_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  glm::vec3 Transform::getUp() const { return glm::vec3(0.0f, 1.0f, 0.0f); }

  glm::vec3 Transform::getFront() const {
    return glm::rotate(glm::conjugate(m_rotation), glm::vec3(0.0f, 0.0f, -1.0f));
  }

  glm::mat4 Transform::getViewMatrix() const {
    glm::mat4 view = glm::mat4();
    view           = glm::mat4_cast(m_rotation);        // rotation
    view           = glm::translate(view, -m_position); // translation
    return view;
  }

  glm::mat4 Transform::getModelMatrix() const {
    // Calculate model from position, rotation and scale
    glm::mat4 model = glm::mat4();
    model           = glm::translate(glm::mat4(1.0f), m_position);
    model *= glm::mat4_cast(m_rotation);
    model = glm::scale(model, m_scale);
    return model;
  }

  void Transform::setPosition(const glm::vec3 &position) { m_position = position; }

  void Transform::translate(const glm::vec3 &translation) { m_position += translation; }

  void Transform::setRotation(const glm::quat &rotation) { m_rotation = rotation; }

  void Transform::setRotation(float pitch, float yaw, float roll) {
    glm::quat qPitch = glm::angleAxis(pitch, glm::vec3{1.f, 0.f, 0.f});
    glm::quat qYaw   = glm::angleAxis(yaw, glm::vec3{0.f, 1.0f, 0.f});
    glm::quat qRoll  = glm::angleAxis(roll, glm::vec3{0.f, 0.f, 1.f});
    m_rotation       = qPitch * qYaw * qRoll;
  }

  void Transform::rotate(const glm::quat &rotation) { m_rotation *= rotation; }

  void Transform::setScale(const glm::vec3 &scale) { m_scale = scale; }

  void Transform::scale(const glm::vec3 &scale) { m_scale += scale; }
} // namespace TritiumEngine::Core