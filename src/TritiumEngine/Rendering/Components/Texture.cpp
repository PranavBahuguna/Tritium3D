#include <TritiumEngine/Rendering/Components/Texture.hpp>

#include <GL/glew.h>

namespace TritiumEngine::Rendering
{
  Texture::Texture(int width, int height, unsigned int target, unsigned int internalFormat,
                   unsigned int format, unsigned int type, const void *data)
      : m_target(target) {
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);
    glTexImage2D(m_target, 0, internalFormat, width, height, 0, format, type, data);
    glBindTexture(m_target, 0);
  }

  Texture::~Texture() { glDeleteTextures(1, &m_id); }

  void Texture::bind() const { glBindTexture(m_target, m_id); }

  void Texture::unbind() const { glBindTexture(m_target, 0); }

  void Texture::selectActiveUnit(unsigned int unit) const { glActiveTexture(GL_TEXTURE0 + unit); }

  void Texture::setParameter(unsigned int param, int value) const {
    glTexParameteri(m_target, param, value);
  }

  void Texture::setParameter(unsigned int param, float value) const {
    glTexParameterf(m_target, param, value);
  }

  void Texture::setParameter(unsigned int param, int *values) const {
    glTexParameteriv(m_target, param, values);
  }

  void Texture::setParameter(unsigned int param, float *values) const {
    glTexParameterfv(m_target, param, values);
  }
} // namespace TritiumEngine::Rendering
