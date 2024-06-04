#pragma once

#include <TritiumEngine/Rendering/Components/Color.hpp>
#include <TritiumEngine/Rendering/Components/Texture.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

namespace TritiumEngine::Rendering
{
  class FrameBuffer {
  public:
    enum class TextureAttachment { COLOR, DEPTH, STENCIL, DEPTH_STENCIL };
    enum class BufferAttachment { DEPTH, DEPTH_STENCIL };

    FrameBuffer(int width, int height);
    FrameBuffer(const FrameBuffer &)            = delete;
    FrameBuffer &operator=(const FrameBuffer &) = delete;
    ~FrameBuffer();

    void attachTexture(TextureAttachment attachment, std::unique_ptr<Texture> texture,
                       unsigned int index = 0);
    void detachTexture(TextureAttachment textureType, unsigned int index = 0);
    void attachRenderBuffer(BufferAttachment attachment);
    void detachRenderBuffer();

    void bind() const;
    void unbind() const;
    Texture* getTextureAttachment(TextureAttachment attachment, unsigned int index = 0) const;
    void clear(Color color) const;
    bool isComplete() const;

  private:
    unsigned int getAttachmentType(TextureAttachment attachment, unsigned int index) const;
    bool validateColorAttachment(int slot) const;

    int m_width;
    int m_height;
    unsigned int m_fbo;
    unsigned int m_rbo;
    std::unordered_map<unsigned int, std::unique_ptr<Texture>> m_textureAttachments;
  };
} // namespace TritiumEngine::Rendering
