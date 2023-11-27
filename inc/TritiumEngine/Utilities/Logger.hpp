#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <TritiumEngine/Utilities/EnumUtils.hpp>

#include <chrono>
#include <ctime>
#include <date.h>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace TritiumEngine::Utilities
{
  enum class LogType : uint32_t {
    DEBUG   = 1u << 0, // Messages for development use
    INFO    = 1u << 1, // General-purpose information messages
    WARNING = 1u << 2, // Alerts user that something has gone wrong
    ERROR   = 1u << 3, // Alerts user that something has gone really wrong

    NONE    = 0,                              // Don't show any log messages
    ALL     = DEBUG | INFO | WARNING | ERROR, // Show all log messages
    NODEBUG = INFO | WARNING | ERROR          // Show all log messages except debug
  };
  ENABLE_ENUM_FLAGS(LogType)

  class Logger {
  public:
    struct Settings {
      static inline LogType levelMask      = LogType::ALL;
      static inline bool showTimestamp     = true;
      static inline const char *timeFormat = "%d-%m-%Y %T";
    };

    /**
     * @brief Outputs a message to console in the following format:
     * [timestamp] [severity level] [formatted message]
     * @param level The severity level this message should be logged at
     * @param msg The message to be logged, can contain format specifiers
     * @param args Additional arguments that can be passed that are used to format the message
     */
    template <class... Args>
    static void log(LogType level, const std::string &msg, Args &&...args) {
      // Check if this message should be logged given its level
      if (!any(Settings::levelMask & level))
        return;

      // Add timestamp
      if (Settings::showTimestamp)
        std::cout << "[" << getTimestamp() << "] ";

      // Add level label and formatted message
      std::cout << getLevelLabel(level) << std::vformat(msg, std::make_format_args(args...))
                << std::endl;
    }

    // Logs a message with DEBUG severity level
    template <class... Args> static void debug(const std::string &msg, Args &&...args) {
      log(LogType::DEBUG, msg, args...);
    }

    // Logs a message with INFO severity level
    template <class... Args> static void info(const std::string &msg, Args &&...args) {
      log(LogType::INFO, msg, args...);
    }

    // Logs a message with WARNING severity level
    template <class... Args> static void warn(const std::string &msg, Args &&...args) {
      log(LogType::WARNING, msg, args...);
    }

    // Logs a message with ERROR severity level
    template <class... Args> static void error(const std::string &msg, Args &&...args) {
      log(LogType::ERROR, msg, args...);
    }

  private:
    Logger() {} // prevent construction of this class

    static inline std::string getTimestamp() {
      auto timeNow        = std::chrono::system_clock::now();
      std::string timeStr = date::format(Settings::timeFormat, timeNow);

      return timeStr;
    }

    static inline std::string getLevelLabel(LogType level) {
      std::string levelLabel = "";

      switch (level) {
      case LogType::DEBUG:
        levelLabel = "[DEBUG]   ";
        break;
      case LogType::INFO:
        levelLabel = "[INFO]    ";
        break;
      case LogType::WARNING:
        levelLabel = "[WARNING] ";
        break;
      case LogType::ERROR:
        levelLabel = "[ERROR]   ";
        break;
      }

      return levelLabel;
    }
  };
} // namespace TritiumEngine::Utilities

#endif // LOGGER_HPP