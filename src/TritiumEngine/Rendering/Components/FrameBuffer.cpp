#include <TritiumEngine/Rendering/Components/FrameBuffer.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  FrameBuffer::FrameBuffer(int width, int height) : m_width(width), m_height(height), m_rbo(0) {
    glGenFramebuffers(1, &m_fbo);
  }

  FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
  }

  void FrameBuffer::attachTexture(TextureAttachment attachment, std::unique_ptr<Texture> texture,
                                  unsigned int index) {
    // Validate color attachment index if used
    if (attachment == TextureAttachment::COLOR && !validateColorAttachment(index))
      return;

    unsigned int textureId      = texture->getId();
    unsigned int attachmentType = getAttachmentType(attachment, index);

    // Store texture and attach to the framebuffer
    m_textureAttachments[attachmentType] = std::move(texture);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::detachTexture(TextureAttachment attachment, unsigned int index) {
    // Validate color attachment index if used
    if (attachment == TextureAttachment::COLOR && !validateColorAttachment(index))
      return;

    unsigned int attachmentType = getAttachmentType(attachment, index);

    // Try removing attachment from storage
    auto it = m_textureAttachments.find(attachmentType);
    m_textureAttachments.erase(it);
  }

  void FrameBuffer::attachRenderBuffer(BufferAttachment renderBufferType) {
    // Delete existing render buffer (if any)
    glDeleteRenderbuffers(1, &m_rbo);

    // Determine properties for depth and depth-stencil renderbuffers respectively
    bool isDepthType            = renderBufferType == BufferAttachment::DEPTH;
    unsigned int internalFormat = isDepthType ? GL_DEPTH_COMPONENT32F : GL_DEPTH24_STENCIL8;
    unsigned int attachment     = isDepthType ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT;

    // Create render buffer and attach to this framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::detachRenderBuffer() { glDeleteRenderbuffers(1, &m_rbo); }

  void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }

  void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  Texture *FrameBuffer::getTextureAttachment(TextureAttachment attachment,
                                             unsigned int index) const {
    // Validate color attachment index if used
    if (attachment == TextureAttachment::COLOR && !validateColorAttachment(index))
      return nullptr;

    unsigned int attachmentType = getAttachmentType(attachment, index);

    auto it = m_textureAttachments.find(attachmentType);
    return it != m_textureAttachments.end() ? it->second.get() : nullptr;
  }

  void FrameBuffer::clear(Color color) const {
    const auto colorVec = ColorUtils::ToNormalizedVec4(color);
    glClearColor(colorVec.r, colorVec.g, colorVec.b, colorVec.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  bool FrameBuffer::isComplete() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    bool isComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return isComplete;
  }

  unsigned int FrameBuffer::getAttachmentType(TextureAttachment attachment,
                                              unsigned int index) const {
    unsigned int attachmentType = 0;
    switch (attachment) {
    case TextureAttachment::COLOR:
      attachmentType = GL_COLOR_ATTACHMENT0 + index;
      break;
    case TextureAttachment::DEPTH:
      attachmentType = GL_DEPTH_ATTACHMENT;
      break;
    case TextureAttachment::STENCIL:
      attachmentType = GL_STENCIL_ATTACHMENT;
      break;
    case TextureAttachment::DEPTH_STENCIL:
      attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
      break;
    }
    return attachmentType;
  }

  bool FrameBuffer::validateColorAttachment(int index) const {
    int maxColorAttachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

    if (index >= maxColorAttachments)
      Logger::warn(
          "[FrameBuffer] Cannot accesss color attachment with index {}, max available color "
          "attachments is {}.",
          index, maxColorAttachments);

    return maxColorAttachments;
  }
} // namespace TritiumEngine::Rendering
