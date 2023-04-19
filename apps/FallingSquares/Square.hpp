#pragma once

#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Entities/Entity.hpp>

#include <glm/glm.hpp>

#include <memory>

using namespace TritiumEngine::Entities;
using namespace TritiumEngine::Rendering;

class Square : public Entity {
public:
  Square(const Transform &startTransform,
         const std::shared_ptr<Renderable> &renderable,
         const std::shared_ptr<Material> &material);

  virtual void Update() override;

private:
  glm::vec3 m_velocity;
};