#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TritiumEngine::Core
{
  class Transform {
  public:
    Transform(const glm::vec3 &position = glm::vec3(0.0f),
              const glm::quat &rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
              const glm::vec3 &scale = glm::vec3(1.0f));

    /** Getter methods for transform properties */
    const glm::vec3 &GetPosition() const;
    const glm::quat &GetRotation() const;
    const glm::vec3 &GetScale() const;

    /** Getter methods for calculated transform vectors and matrices */
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetFront() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetModelMatrix() const;

    /** Setter methods for transform properties */
    void SetPosition(const glm::vec3 &position);
    void Translate(const glm::vec3 &translation);
    void SetRotation(const glm::quat &rotation);
    void Rotate(const glm::quat &rotation);
    void SetScale(const glm::vec3 &scale);
    void Scale(const glm::vec3 &scale);

  private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
  };
} // namespace TritiumEngine::Core