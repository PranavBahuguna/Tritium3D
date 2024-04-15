#pragma once

#include <TritiumEngine/Core/ResourceLoader.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <filesystem>
#include <memory>
#include <unordered_map>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  template <typename T, typename U>
  using IsResourceLoaderType =
      typename std::enable_if_t<std::is_base_of_v<ResourceLoader<T>, U>, bool>;

  template <class T> class ResourceManager {
  public:
    /**
     * @brief Registers a resource loader (loading type T) into the manager
     * @tparam U Type of resource loader, must provide resources of type T
     * @param loader The resource loader class to be registered
     * @param rootDir The root directory where all objects to construct of the given type are
     * @param Args Additional parameters required to construct the system stored
     */
    template <class U, typename... Args, IsResourceLoaderType<T, U> = true>
    static void registerLoader(const std::string &rootDir, Args &&...args) {
      if (m_isRegistered) {
        Logger::warn(
            "[ResourceManager] Loader of type {} already registered for this resource manager.",
            typeid(T).name());
        return;
      }

      m_isRegistered = true;
      m_loader       = std::make_unique<U>(std::forward<Args>(args)...);
      m_rootDir      = rootDir;
    }

    /**
     * @brief Loads and constructs a new resource of type T from file, returns existing one if
     * already loaded
     * @param filePath The file path of the resource to load
     * @param forceReload If true, will forcibly load the resource again and overwrite an existing
     * resource
     * @param optional If true, will suppress any warnings or errors when attempting to obtain
     * resource
     * @return Shared pointer to resource of given type, nullptr if resource file path isn't found
     */
    static std::shared_ptr<T> get(const std::string &filePath, bool forceReload = false,
                                  bool optional = false) {
      if (!checkIfRegistered() || filePath.empty())
        return nullptr;

      std::string totalFilePath     = m_rootDir + filePath;
      auto &foundItem               = m_resourceMap[totalFilePath];
      std::shared_ptr<T> returnItem = nullptr;

      // Supress warnings if optional
      LogType loggerLevel = Logger::Settings::levelMask;
      if (optional)
        Logger::Settings::levelMask &= ~(LogType::ERROR | LogType::WARNING);

      if (foundItem.expired() || forceReload) {
        if (std::filesystem::exists(totalFilePath)) {
          // Load from file path
          returnItem = std::shared_ptr<T>(m_loader->load(totalFilePath));
          foundItem  = returnItem;
        } else {
          Logger::error("[ResourceManager] File {} not found", totalFilePath);
        }
      } else {
        returnItem = foundItem.lock();
      }

      // Restore original logger settings
      if (optional)
        Logger::Settings::levelMask = loggerLevel;

      return returnItem;
    }

    /**
     * @brief Checks if a given file exists
     * @param filePath The file path of a resource to find
     */
    static bool fileExists(const std::string &filePath) {
      if (!checkIfRegistered() || filePath.empty())
        return false;

      std::string totalFilePath = m_rootDir + filePath;
      return std::filesystem::exists(totalFilePath);
    }

    /**
     * @brief Tries locating a stored resource of type T given its file path
     * @return True if resource is found
     */
    static bool find(const std::string &filePath) {
      if (!checkIfRegistered() || filePath.empty())
        return false;

      std::string totalFilePath = m_rootDir + filePath;
      auto it                   = m_resourceMap.find(totalFilePath);
      if (it == m_resourceMap.end())
        return false;

      return !it->second.expired();
    }

  private:
    static bool checkIfRegistered() {
      if (!m_isRegistered)
        Logger::warn("[ResourceManager] Resource manager for type {} is not registered!",
                     typeid(T).name());

      return m_isRegistered;
    }

    static inline bool m_isRegistered   = false;
    static inline std::string m_rootDir = "";
    static inline std::unordered_map<std::string, std::weak_ptr<T>> m_resourceMap;
    static inline std::unique_ptr<ResourceLoader<T>> m_loader;
  };
} // namespace TritiumEngine::Core