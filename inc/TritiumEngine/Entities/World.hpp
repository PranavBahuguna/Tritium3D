#pragma once

#include <TritiumEngine/Entities/Entity.hpp>

namespace TritiumEngine::Entities
{
  class World {
  public:
    const std::vector<Entity *> &GetEntities() const;

    void AddEntity(Entity *entity);
    void ClearEntities();
    void Update(float deltaTime);

  private:
    std::vector<Entity *> m_entities;
  };
} // namespace TritiumEngine::Entities