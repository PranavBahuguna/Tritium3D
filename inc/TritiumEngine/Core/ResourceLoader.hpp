#pragma once

#include <string>

namespace TritiumEngine::Core
{
  template <class T> class ResourceLoader {
  public:
    virtual T *load(const std::string &filePath) = 0;
  };
} // namespace TritiumEngine::Core