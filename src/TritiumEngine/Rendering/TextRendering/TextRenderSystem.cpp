#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Font.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>
#include <TritiumEngine/Rendering/TextRendering/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace TritiumEngine::Rendering::TextRendering
{
  TextRenderSystem::TextRenderSystem(BlendOptions blendOptions) : RenderSystem(blendOptions) {}

  void TextRenderSystem::draw(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<Text, Transform, Shader, Color>().each(
        [&](auto entity, Text &text, Transform &transform, Shader &shader, Color &color) {
          // Apply properties to shader
          shaderManager.use(shader.id);
          shaderManager.setMatrix4("mvp",
                                   transform.getModelMatrix() * camera.calcProjectionViewMatrix());
          shaderManager.setVector4("color", ColorUtils::ToNormalizedVec4(color));

          // Starting x/y position current character in text string
          glm::vec2 startPos = getStartPosition(text);

          glBindVertexArray(text.getVao());
          glActiveTexture(GL_TEXTURE0);

          // Iterate and draw each character
          for (const char &c : text.text) {
            const auto &ch = text.getFont()->characters[c];

            // Calculate position and size for the given character
            float scale = text.scale;
            float xPos  = startPos.x + ch.bearing.x * scale;
            float yPos  = startPos.y - (ch.size.y - ch.bearing.y) * scale;
            float w     = ch.size.x * scale;
            float h     = ch.size.y * scale;

            // Add vertex data and texture coordinates
            float vertices[4][4] = {
                {xPos, yPos, 0.0f, 1.0f},
                {xPos + w, yPos, 1.0f, 1.0f},
                {xPos, yPos + h, 0.0f, 0.0f},
                {xPos + w, yPos + h, 1.0f, 0.0f},
            };

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, text.getVbo());
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // Advance x-position for next glyph
            startPos.x += (ch.advance >> 6) * scale;
          }
        });
  }

  glm::vec2 TextRenderSystem::getStartPosition(const Text &text) const {
    glm::vec2 startPos = glm::vec2();
    switch (text.align) {
    case Text::Alignment::TOP_LEFT:
      startPos = {0.f, -text.getPixelHeight()};
      break;
    case Text::Alignment::TOP_CENTER:
      startPos = {-text.getPixelWidth() * 0.5f, -text.getPixelHeight()};
      break;
    case Text::Alignment::TOP_RIGHT:
      startPos = {-text.getPixelWidth(), -text.getPixelHeight()};
      break;
    case Text::Alignment::CENTER_LEFT:
      startPos = {0.f, -text.getPixelHeight() * 0.5f};
      break;
    case Text::Alignment::CENTER:
      startPos = {-text.getPixelWidth() * 0.5f, -text.getPixelHeight() * 0.5f};
      break;
    case Text::Alignment::CENTER_RIGHT:
      startPos = {-text.getPixelWidth(), -text.getPixelHeight() * 0.5f};
      break;
    case Text::Alignment::BOTTOM_LEFT:
      startPos = {0.f, 0.f};
      break;
    case Text::Alignment::BOTTOM_CENTER:
      startPos = {-text.getPixelWidth() * 0.5f, 0.f};
      break;
    case Text::Alignment::BOTTOM_RIGHT:
      startPos = {-text.getPixelWidth(), 0.f};
      break;
    }

    return startPos;
  }
} // namespace TritiumEngine::Rendering::TextRendering