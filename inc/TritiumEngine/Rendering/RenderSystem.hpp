#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <TritiumEngine/Core/System.hpp>

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class ShaderManager;
  class Camera;

  class RenderSystem : public System {
  public:
    enum class DrawMode { STANDARD, INSTANCED };

    void draw(const Camera &camera) const;

    void update(float dt) override;
    void setDrawMode(DrawMode drawMode);

  private:
    void drawStandard(const Camera &camera) const;
    void drawInstanced(const Camera &camera) const;

    DrawMode m_drawMode = DrawMode::STANDARD;
  };
} // namespace TritiumEngine::Rendering

#endif // RENDER_SYSTEM_HPP