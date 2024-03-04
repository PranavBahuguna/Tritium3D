#include <TritiumEngine/Rendering/TextRendering/Font.hpp>

#include <GL/glew.h>

#include <string>

using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Rendering::TextRendering
{
  class Text {
  public:
    enum class Alignment {
      TOP_LEFT,
      TOP_CENTER,
      TOP_RIGHT,
      CENTER_LEFT,
      CENTER,
      CENTER_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_CENTER,
      BOTTOM_RIGHT
    };

    Text(const std::string &text, const std::string &font, float scale = 1.f,
         Alignment alignment = Alignment::BOTTOM_LEFT);
    ~Text();

    float getPixelWidth() const;
    float getPixelHeight() const;
    GLuint getVao() const { return m_vao; }
    GLuint getVbo() const { return m_vbo; }

    std::string text;
    std::string font;
    float scale;
    Alignment align;

  private:
    GLuint m_vao;
    GLuint m_vbo;
  };
} // namespace TritiumEngine::Rendering::TextRendering