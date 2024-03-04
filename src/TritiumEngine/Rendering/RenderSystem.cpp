#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Core/Transform.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/Color.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Text.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <entt/entity/registry.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering::TextRendering;

namespace TritiumEngine::Rendering
{
  void RenderSystem::update(float dt) {
    const Camera &camera = m_app->registry.get<Camera>(m_app->sceneManager.getCurrentSceneEntity());
    draw(camera);
    drawInstanced(camera);
    drawText(camera);
  }

  void RenderSystem::draw(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<Renderable, Transform, Shader, Color>().each(
        [&](auto entity, Renderable &renderable, Transform &transform, Shader &shader,
            Color &color) {
          // Apply properties to shader
          shaderManager.use(shader.id);
          shaderManager.setMatrix4("model", transform.getModelMatrix());
          shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
          shaderManager.setVector4("color", color.asNormalizedVec4());

          GLuint vao         = renderable.getVao();
          GLint vertexStride = renderable.getVertexStride();
          GLsizei nVertices  = renderable.getNumVertices();
          GLsizei nIndices   = renderable.getNumIndices();
          GLenum renderMode  = renderable.getRenderMode();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
          else
            glDrawArrays(renderMode, 0, nVertices / vertexStride);
        });
  }

  void RenderSystem::drawInstanced(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<InstancedRenderable, Shader>().each(
        [&](auto entity, InstancedRenderable &renderable, Shader &shader) {
          GLuint vao          = renderable.getVao();
          GLint vertexStride  = renderable.getVertexStride();
          GLsizei nVertices   = renderable.getNumVertices();
          GLsizei nIndices    = renderable.getNumIndices();
          GLsizei nInstances  = renderable.getNumInstances();
          GLenum renderMode   = renderable.getRenderMode();
          uint32_t instanceId = renderable.getInstanceId();

          shaderManager.use(shader.id);
          shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());

          // Update model matrices
          GLsizei index      = 0;
          auto instancesView = m_app->registry.view<Transform, InstancedRenderableTag>();
          m_app->registry.view<Transform, Color, InstancedRenderableTag>().each(
              [&](auto entity, Transform &transform, Color &color, InstancedRenderableTag &tag) {
                if (index < nInstances && tag.value == instanceId)
                  renderable.setInstanceData(index++, {transform.getModelMatrix(), color.value});
              });
          renderable.updateInstanceDataBuffer();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElementsInstanced(renderMode, nIndices, GL_UNSIGNED_INT, 0, nInstances);
          else
            glDrawArraysInstanced(renderMode, 0, nVertices / vertexStride, nInstances);
        });
  }

  void RenderSystem::drawText(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<Text, Transform, Shader, Color>().each(
        [&](auto entity, Text &text, Transform &transform, Shader &shader, Color &color) {
          // Apply properties to shader
          shaderManager.use(shader.id);
          shaderManager.setMatrix4("mvp",
                                   transform.getModelMatrix() * camera.calcProjectionViewMatrix());
          shaderManager.setVector4("color", color.asNormalizedVec4());

          const auto &font = ResourceManager<Font>::get(text.font + ".ttf");

          // Starting x/y position current character in text string
          glm::vec2 startPos = glm::vec2(0.f, 0.f);
          switch (text.align) {
          case Text::Alignment::TopLeft:
            startPos = {0.f, -text.getPixelHeight()};
            break;
          case Text::Alignment::TopCenter:
            startPos = {-text.getPixelWidth() * 0.5f, -text.getPixelHeight()};
            break;
          case Text::Alignment::TopRight:
            startPos = {-text.getPixelWidth(), -text.getPixelHeight()};
            break;
          case Text::Alignment::CenterLeft:
            startPos = {0.f, -text.getPixelHeight() * 0.5f};
            break;
          case Text::Alignment::Center:
            startPos = {-text.getPixelWidth() * 0.5f, -text.getPixelHeight() * 0.5f};
            break;
          case Text::Alignment::CenterRight:
            startPos = {-text.getPixelWidth(), -text.getPixelHeight() * 0.5f};
            break;
          case Text::Alignment::BottomLeft:
            startPos = {0.f, 0.f};
            break;
          case Text::Alignment::BottomCenter:
            startPos = {-text.getPixelWidth() * 0.5f, 0.f};
            break;
          case Text::Alignment::BottomRight:
            startPos = {-text.getPixelWidth(), 0.f};
            break;
          }

          glBindVertexArray(text.getVao());
          glActiveTexture(GL_TEXTURE0);

          // Iterate and draw each character
          for (const char &c : text.text) {
            const auto &ch = font->characters[c];

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
} // namespace TritiumEngine::Rendering