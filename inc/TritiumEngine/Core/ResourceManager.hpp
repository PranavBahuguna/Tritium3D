#pragma once

#include <TritiumEngine/Core/ResourceLoaderFactory.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <memory>
#include <unordered_map>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  template <class T> class ResourceManager {
  public:
    /**
     * Registers a resource loader factory of type T into the manager
     *
     * @param loaderFactory The loader factory to be registered.
     * @param rootDir The root directory where all objects to construct of the given type are
     * stored.
     */
    static void Register(std::unique_ptr<ResourceLoaderFactory<T>> loaderFactory,
                         const std::string &rootDir) {
      if (m_isRegistered) {
        Logger::Log(LogType::WARNING,
                    "Factory of type {} already registered for this resource manager.",
                    typeid(T).name());
        return;
      }

      m_isRegistered  = true;
      m_loaderFactory = std::move(loaderFactory);
      m_rootDir       = rootDir;
    }

    /**
     * Loads and constructs a new resource of type T from file, returns existing one if already
     * loeaded.
     *
     * @param filePath The file path of the resource to load.
     * @param forceReload If true, will forcibly load the resource again and overwrite an existing
     * resource.
     * @return Shared pointer to resource of given type, nullptr if resource file path isn't found.
     */
    static std::shared_ptr<T> Get(const std::string &filePath, bool forceReload = false) {
      if (!CheckIfRegistered() || filePath.empty())
        return nullptr;

      std::string totalFilePath     = m_rootDir + filePath;
      auto &foundItem               = m_resourceMap[totalFilePath];
      std::shared_ptr<T> returnItem = nullptr;

      if (foundItem.expired() || forceReload) {
        returnItem = std::shared_ptr<T>(m_loaderFactory->Load(totalFilePath));
        foundItem  = returnItem;
      } else {
        returnItem = foundItem.lock();
      }

      return returnItem;
    }

    /**
     * Tries locating a stored resource of type T given its file path.
     *
     * @return True if resource is found.
     */
    static bool Find(const std::string &filePath) {
      if (!CheckIfRegistered() || filePath.empty())
        return false;

      std::string totalFilePath = m_rootDir + filePath;
      auto it                   = m_resourceMap.find(totalFilePath);
      if (it == m_resourceMap.end())
        return false;

      return !it->second.expired();
    }

  private:
    static bool CheckIfRegistered() {
      if (!m_isRegistered)
        Logger::Log(LogType::WARNING, "Resource manager for type {} is not registered!",
                    typeid(T).name());

      return m_isRegistered;
    }

    static inline bool m_isRegistered   = false;
    static inline std::string m_rootDir = "";
    static inline std::unordered_map<std::string, std::weak_ptr<T>> m_resourceMap;
    static inline std::unique_ptr<ResourceLoaderFactory<T>> m_loaderFactory;
  };
} // namespace TritiumEngine::Core