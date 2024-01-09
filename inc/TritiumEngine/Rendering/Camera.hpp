#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <TritiumEngine/Core/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  struct Camera {
    enum class ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

    Camera(ProjectionType projection, float frameWidth, float frameHeight, float nearPlane,
           float farPlane, float fov = glm::radians(60.0f),
           const Transform &transform = Transform())
        : projection(projection), width(frameWidth), height(frameHeight), nearPlane(nearPlane),
          farPlane(farPlane), fov(fov), transform(transform) {}

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
    float fov;
    Transform transform;
  };
} // namespace TritiumEngine::Rendering

#endif // CAMERA_HPP