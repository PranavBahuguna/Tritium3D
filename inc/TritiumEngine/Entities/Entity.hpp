#pragma once

#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Material.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>

#include <memory>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Entities
{
  typedef std::vector<std::shared_ptr<Renderable>> Renderables;
  typedef std::vector<std::shared_ptr<Material>> Materials;

  struct Entity {
    Entity(const Transform &transform, const Renderables &renderables, const Materials &materials) {
      this->transform   = transform;
      this->renderables = renderables;
      this->materials   = materials;
    }

    Entity(const Transform &transform, const std::shared_ptr<Renderable> &renderable,
           const std::shared_ptr<Material> &material)
        : Entity(transform, Renderables{renderable}, Materials{material}) {}

    virtual void Init() {}
    virtual void Update() {}

    Transform transform;
    Renderables renderables;
    Materials materials;
  };
} // namespace TritiumEngine::Entities