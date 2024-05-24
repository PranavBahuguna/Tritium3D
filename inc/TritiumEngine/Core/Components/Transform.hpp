#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TritiumEngine::Core
{
  struct Transform {
  public:
    constexpr static glm::vec3 RIGHT = {1.f, 0.f, 0.f};
    constexpr static glm::vec3 LEFT  = {-1.f, 0.f, 0.f};
    constexpr static glm::vec3 UP    = {0.f, 1.f, 0.f};
    constexpr static glm::vec3 DOWN  = {0.f, -1.f, 0.f};
    constexpr static glm::vec3 BACK  = {0.f, 0.f, 1.f};
    constexpr static glm::vec3 FRONT = {0.f, 0.f, -1.f};

    Transform(const glm::vec3 &position = glm::vec3(0.f),
              const glm::quat &rotation = glm::quat(1.f, 0.f, 0.f, 0.f),
              const glm::vec3 &scale    = glm::vec3(1.f))
        : position(position), rotation(rotation), scale(scale) {}

    Transform(const glm::vec3 &position, const glm::vec3 &rotationEuler,
              const glm::vec3 &scale = glm::vec3(1.f))
        : position(position), rotation(rotationEuler), scale(scale) {
    }

    glm::vec3 getRight() const { return glm::rotate(glm::conjugate(rotation), RIGHT); }

    glm::vec3 getUp() const { return glm::rotate(glm::conjugate(rotation), UP); }

    glm::vec3 getFront() const { return glm::rotate(glm::conjugate(rotation), FRONT); }

    glm::mat4 getViewMatrix() const { return glm::lookAt(position, position + getFront(), UP); }

    glm::mat4 getModelMatrix() const {
      // Calculate model from position, rotation and scale
      auto model = glm::mat4(1.0f);
      model      = glm::translate(model, position);
      model      = model * glm::mat4_cast(rotation);
      model      = glm::scale(model, scale);
      return model;
    }

    void setRotation(float pitch, float yaw, float roll) {
      auto qPitch = glm::angleAxis(pitch, LEFT);
      auto qYaw   = glm::angleAxis(yaw, UP);
      auto qRoll  = glm::angleAxis(roll, FRONT);
      rotation    = qPitch * qYaw * qRoll;
    }

    void rotate(float pitch, float yaw, float roll) {
      auto qPitch = glm::angleAxis(pitch, LEFT);
      auto qYaw   = glm::angleAxis(yaw, UP);
      auto qRoll  = glm::angleAxis(roll, FRONT);
      rotation *= qPitch * qYaw * qRoll;
    }

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
  };
} // namespace TritiumEngine::Core