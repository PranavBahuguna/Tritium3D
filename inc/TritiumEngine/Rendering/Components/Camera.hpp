#pragma once

#include <TritiumEngine/Core/Components/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera {
    enum class Projection { ORTHOGRAPHIC, PERSPECTIVE };

    Camera(Projection projection, float frameWidth, float frameHeight,
           const Transform &transform = {}, float nearPlane = 0.1f, float farPlane = 100.f,
           float fov = glm::radians(60.f))
        : projection(projection), width(frameWidth), height(frameHeight), transform(transform),
          nearPlane(nearPlane), farPlane(farPlane), fov(fov) {}

    float getAspectRatio() const { return width / height; }

    glm::mat4 calcProjectionMatrix() const {
      if (projection == Projection::ORTHOGRAPHIC) {
        float left   = -width * transform.scale.x * 0.5f + transform.position.x;
        float right  = width * transform.scale.x * 0.5f + transform.position.x;
        float bottom = -height * transform.scale.y * 0.5f + transform.position.y;
        float top    = height * transform.scale.y * 0.5f + transform.position.y;
        return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
      } else {
        return glm::perspective(fov, getAspectRatio(), nearPlane, farPlane);
      }
    }

    glm::mat4 calcProjectionViewMatrix() const {
      return calcProjectionMatrix() * transform.getViewMatrix();
    }

    Projection projection;
    float width;
    float height;
    Transform transform;
    float nearPlane;
    float farPlane;
    float fov;
  };
} // namespace TritiumEngine::Rendering