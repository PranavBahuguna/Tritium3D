#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/TextRendering/Components/Text.hpp>
#include <TritiumEngine/Rendering/TextRendering/FontLoader.hpp>

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  Text::Text(const std::string &text, const std::string &font, float scaleFactor, Alignment alignment)
      : text(text), scaleFactor(scaleFactor), align(alignment),
        m_font(ResourceManager<Font>::get(font + ".ttf")) {
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
    float width = 0.f;
    for (const char &c : text)
      width += m_font->characters[c].advance >> 6;

    return width * scaleFactor;
  }

  float Text::getPixelHeight() const {
    // Obtains the greatest pixel height from all characters in the text string
    int maxHeight = 0;
    for (const char &c : text)
      maxHeight = std::max(m_font->characters[c].bearing.y, maxHeight);

    return maxHeight * scaleFactor;
  }
} // namespace TritiumEngine::Rendering