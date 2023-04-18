#include <TritiumEngine/Core/Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Core
{
  Transform::Transform(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale)
      : m_position(position), m_rotation(rotation), m_scale(scale) {}

  const glm::vec3 &Transform::GetPosition() const { return m_position; }

  const glm::quat &Transform::GetRotation() const { return m_rotation; }

  const glm::vec3 &Transform::GetScale() const { return m_scale; }

  glm::vec3 Transform::GetRight() const {
    return glm::rotate(glm::conjugate(m_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  glm::vec3 Transform::GetUp() const { return glm::vec3(0.0f, 1.0f, 0.0f); }

  glm::vec3 Transform::GetFront() const {
    return glm::rotate(glm::conjugate(m_rotation), glm::vec3(0.0f, 0.0f, -1.0f));
  }

  glm::mat4 Transform::GetViewMatrix() const {
    glm::mat4 view = glm::mat4();
    view = glm::mat4_cast(m_rotation);        // rotation
    view = glm::translate(view, -m_position); // translation
    return view;
  }

  glm::mat4 Transform::GetModelMatrix() const {
    // Calculate model from position, rotation and scale
    glm::mat4 model = glm::mat4();
    model = glm::translate(glm::mat4(1.0f), m_position);
    model *= glm::mat4_cast(m_rotation);
    model = glm::scale(model, m_scale);
    return model;
  }

  void Transform::SetPosition(const glm::vec3 &position) { m_position = position; }

  void Transform::Translate(const glm::vec3 &translation) { m_position += translation; }

  void Transform::SetRotation(const glm::quat &rotation) { m_rotation = rotation; }

  void Transform::Rotate(const glm::quat &rotation) { m_rotation *= rotation; }

  void Transform::SetScale(const glm::vec3 &scale) { m_scale = scale; }

  void Transform::Scale(const glm::vec3 &scale) { m_scale += scale; }
} // namespace TritiumEngine::Core