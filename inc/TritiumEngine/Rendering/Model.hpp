#ifndef MODEL_HPP
#define MODEL_HPP

#include <TritiumEngine/Rendering/Renderable.hpp>

#include <memory>

namespace TritiumEngine::Rendering
{
  class Model {
  public:
    void render() const {
      for (const auto &renderable : renderables)
        renderable->render();
    }

    std::vector<std::shared_ptr<Renderable>> renderables;
  };
} // namespace TritiumEngine::Rendering

#endif // MODEL_HPP
