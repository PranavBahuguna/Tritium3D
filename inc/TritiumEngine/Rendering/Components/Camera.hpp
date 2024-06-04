#pragma once

#include <TritiumEngine/Core/Components/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera {
    enum class Projection { ORTHOGRAPHIC, PERSPECTIVE };

    Camera(Projection projection, float frameWidth, float frameHeight, float nearPlane = 0.1f,
           float farPlane = 100.f, const Transform &transform = {}, float fov = glm::radians(60.f))
        : projection(projection), width(frameWidth), height(frameHeight), nearPlane(nearPlane),
          farPlane(farPlane), transform(transform), fov(fov) {}

    float getAspectRatio() const { return width / height; }

    glm::mat4 calcProjectionMatrix() const {
      return projection == Projection::ORTHOGRAPHIC
                 ? glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f)
                 : glm::perspective(fov, getAspectRatio(), nearPlane, farPlane);
    }

    glm::mat4 calcProjectionViewMatrix() const {
      return calcProjectionMatrix() * transform.getViewMatrix();
    }

    Projection projection;
    float width;
    float height;
    float nearPlane;
    float farPlane;
    Transform transform;
    float fov;
  };
} // namespace TritiumEngine::Rendering