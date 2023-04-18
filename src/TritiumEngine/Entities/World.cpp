#include <TritiumEngine/Entities/World.hpp>

namespace TritiumEngine::Entities
{
  /**
   * Getter method for all stored entities.
   *
   * @return List of all stored entities
   */
  const std::vector<Entity *> &World::GetEntities() const { return m_entities; }

  /**
   * Adds a new entity to the world.
   *
   * @param entity The entity to be added.
   */
  void World::AddEntity(Entity *entity) { m_entities.push_back(entity); }

  /** Remove and delete all entities. */
  void World::ClearEntities() {
    for (auto &entity : m_entities)
      delete entity;

    m_entities.clear();
  }

  /**
   * Updates all managed entities
   *
   * @param deltaTime Elapsed time since last update
   */
  void World::Update(float deltaTime) {
    for (auto &entity : m_entities)
      entity->Update(deltaTime);
  }
} // namespace TritiumEngine::Entities