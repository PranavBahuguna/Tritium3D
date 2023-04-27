#include <TritiumEngine/Entities/Entity.hpp>
#include <TritiumEngine/Entities/World.hpp>
#include <TritiumEngine/Utilities/Timer.hpp>

#include <stdexcept>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Entities
{
  World::World() : m_entities(), m_nEntities(0), m_isInitialised(false) {}

  World::~World() { ClearEntities(); }

  /**
   * Getter method for all stored entities.
   *
   * @return Pointer to all stored entities
   */
  const EntityList World::GetEntities() const { return m_entities; }

  /**
   * Adds a new entity to the world.
   *
   * @param entity The entity to be added.
   */
  void World::AddEntity(Entity *entity) { m_entities[m_nEntities++] = entity; }

  /** Remove and delete all entities. */
  void World::ClearEntities() {
    for (size_t i = 0; i < m_nEntities; ++i)
      delete m_entities[i];

    m_nEntities = 0;
  }

  /** Initialises the world and all entities. */
  void World::Init() {
    Timer::Start();
    for (size_t i = 0; i < m_nEntities; ++i)
      m_entities[i]->Init();

    m_isInitialised = true;
  }

  /**
   * Updates all managed entities
   *
   * @param deltaTime Elapsed time since last update
   */
  void World::Update() {
    if (!m_isInitialised)
      throw std::runtime_error("World must be initialised first before update!");

    Timer::Update();
    for (size_t i = 0; i < m_nEntities; ++i)
      m_entities[i]->Update();
  }

  /**
   * Getter method for number of stored entities
   *
   * @return Number of stored entities
   */
  size_t World::GetNumEntities() const { return m_nEntities; }
} // namespace TritiumEngine::Entities