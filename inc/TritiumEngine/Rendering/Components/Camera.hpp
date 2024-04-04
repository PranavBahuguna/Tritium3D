#pragma once

#include <TritiumEngine/Core/Components/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera {
    enum class ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

    Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
           float farPlane, const Transform &transform = Transform(), float fov = glm::radians(60.f))
        : projection(projection), width(frameWidth), height(frameHeight), nearPlane(nearPlane),
          farPlane(farPlane), transform(transform), fov(fov) {}

    float getAspectRatio() const { return width / height; }

    glm::mat4 calcProjectionMatrix() const {
      return projection == ProjectionType::ORTHOGRAPHIC
                 ? glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f)
                 : glm::perspective(fov, getAspectRatio(), nearPlane, farPlane);
    }

    glm::mat4 calcProjectionViewMatrix() const {
      return calcProjectionMatrix() * transform.getViewMatrix();
    }

    ProjectionType projection;
    float width;
    float height;
    float nearPlane;
    float farPlane;
    Transform transform;
    float fov;
  };
} // namespace TritiumEngine::Rendering