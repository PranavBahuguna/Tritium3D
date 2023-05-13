#ifndef WORLD_HPP
#define WORLD_HPP

#define MAX_ENTITIES 5000

#include <array>

namespace TritiumEngine::Entities
{
  struct Entity;

  typedef std::array<Entity *, MAX_ENTITIES> EntityList;

  class World {
  public:
    World();
    ~World();

    const EntityList GetEntities() const;
    size_t GetNumEntities() const;

    void AddEntity(Entity *entity);
    void ClearEntities();
    void Init();
    void Update(float deltaTime);

  private:
    EntityList m_entities;
    size_t m_nEntities;
    bool m_isInitialised;
  };
} // namespace TritiumEngine::Entities

#endif // WORLD_HPP