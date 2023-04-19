#include "Square.hpp"

#include <TritiumEngine/Utilities/Timer.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#define GRAVITY 0.04f
#define MAX_SPEED 0.25f

using namespace TritiumEngine::Utilities;

Square::Square(const Transform &transform, const std::shared_ptr<Renderable> &renderable,
               const std::shared_ptr<Material> &material)
    : Entity(transform, renderable, material), m_velocity(0.0f) {}

void Square::Update() {
  // Calculate velocity and update square's position
  m_velocity = glm::vec3(m_velocity.x, m_velocity.y - Timer::DeltaTime() * GRAVITY, 0.0f);
  if (glm::length(m_velocity) > MAX_SPEED)
    m_velocity = glm::normalize(m_velocity) * MAX_SPEED;

  transform.Translate(m_velocity);
}