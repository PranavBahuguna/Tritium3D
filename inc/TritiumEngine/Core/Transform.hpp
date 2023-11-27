#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TritiumEngine::Core
{
  class Transform {
  public:
    Transform(const glm::vec3 &position = glm::vec3(0.0f),
              const glm::quat &rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
              const glm::vec3 &scale    = glm::vec3(1.0f));

    // Getter methods for transform properties
    const glm::vec3 &getPosition() const;
    const glm::quat &getRotation() const;
    const glm::vec3 &getScale() const;

    // Getter methods for calculated transform vectors and matrices
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
    glm::vec3 getFront() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getModelMatrix() const;

    // Setter methods for transform properties
    void setPosition(const glm::vec3 &position);
    void translate(const glm::vec3 &translation);
    void setRotation(const glm::quat &rotation);
    void rotate(const glm::quat &rotation);
    void setScale(const glm::vec3 &scale);
    void scale(const glm::vec3 &scale);

  private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
  };
} // namespace TritiumEngine::Core

#endif // TRANSFORM_HPP