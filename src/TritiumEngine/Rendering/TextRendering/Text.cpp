#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/TextRendering/FontLoaderFactory.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>

namespace TritiumEngine::Rendering::TextRendering
{
  Text::Text(const std::string &text, const std::string &font, float scale, Alignment alignment)
      : text(text), font(font), scale(scale), align(alignment) {
    // Bind vertex array object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Bind vertex data buffer
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
  }

  Text::~Text() {
    // Delete all buffer and vertex data
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
  }

  float Text::getPixelWidth() const {
    // Obtains the total pixel width of the text string
    const auto &fontResource = ResourceManager<Font>::get(font + ".ttf");
    float width              = 0.f;
    for (const char &c : text)
      width += fontResource->characters[c].advance >> 6;

    return width * scale;
  }

  float Text::getPixelHeight() const {
    // Obtains the greatest pixel height from all characters in the text string
    const auto &fontResource = ResourceManager<Font>::get(font + ".ttf");
    int maxHeight            = 0;
    for (const char &c : text)
      maxHeight = std::max(fontResource->characters[c].bearing.y, maxHeight);

    return maxHeight * scale;
  }
} // namespace TritiumEngine::Rendering::TextRendering