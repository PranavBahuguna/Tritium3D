#pragma once

namespace TritiumEngine::Rendering
{
  class Texture {
  public:
    Texture(int width, int height, unsigned int target, unsigned int internalFormat,
            unsigned int format, unsigned int type, const void *data = nullptr);
    Texture(const Texture &)            = delete;
    Texture &operator=(const Texture &) = delete;
    ~Texture();

    void bind() const;
    void unbind() const;
    void selectActiveUnit(unsigned int unit) const;
    void setParameter(unsigned int param, int value) const;
    void setParameter(unsigned int param, float value) const;
    void setParameter(unsigned int param, int *values) const;
    void setParameter(unsigned int param, float *values) const;
    unsigned int getId() const { return m_id; }
    unsigned int getTarget() const { return m_target; }

  private:
    unsigned int m_id;
    unsigned int m_target;
  };
} // namespace TritiumEngine::Rendering