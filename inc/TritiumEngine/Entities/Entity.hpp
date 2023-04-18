#pragma once

#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>

#include <memory>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Entities
{
  struct Entity {
    Entity(const Transform &transform, const std::shared_ptr<Renderable> &renderable,
           const std::shared_ptr<Material> &material) {
      this->transform = transform;
      this->renderables = {renderable};
      this->materials = {material};
    }

    Entity(const Transform &transform, const std::vector<std::shared_ptr<Renderable>> &renderables,
           const std::vector<std::shared_ptr<Material>> &materials) {
      this->transform = transform;
      this->renderables = renderables;
      this->materials = materials;
    }

    virtual void Update(float deltaTime) {}

    Transform transform;
    std::vector<std::shared_ptr<Renderable>> renderables;
    std::vector<std::shared_ptr<Material>> materials;
  };
} // namespace TritiumEngine::Entities