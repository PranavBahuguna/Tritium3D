#ifndef RESOURCE_LOADER_FACTORY_HPP
#define RESOURCE_LOADER_FACTORY_HPP

#include <string>

namespace TritiumEngine::Core
{
  template <class T> class ResourceLoaderFactory {
  public:
    virtual T *Load(const std::string &filePath) = 0;
  };
} // namespace TritiumEngine::Core

#endif // RESOURCE_LOADER_FACTORY_HPP