#pragma once

#include <GL/glew.h>

#include <memory>
#include <string>

namespace TritiumEngine::Rendering::TextRendering
{
  struct Font;

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
    Font *getFont() const { return m_font.get(); }
    GLuint getVao() const { return m_vao; }
    GLuint getVbo() const { return m_vbo; }

    std::string text;
    float scale;
    Alignment align;

  private:
    std::shared_ptr<Font> m_font;
    GLuint m_vao;
    GLuint m_vbo;
  };
} // namespace TritiumEngine::Rendering::TextRendering