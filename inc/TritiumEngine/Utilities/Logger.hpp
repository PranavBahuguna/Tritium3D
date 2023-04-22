#pragma once

#include <chrono>
#include <ctime>
#include <date.h>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace TritiumEngine::Utilities
{
  enum LogType : uint32_t {
    // Severity levels
    DEBUG   = 1u << 0,
    INFO    = 1u << 1,
    WARNING = 1u << 2,
    ERROR   = 1u << 3,
    // Enabled levels bitmasks
    NONE    = 0,
    ALL     = DEBUG | INFO | WARNING | ERROR,
    NODEBUG = INFO | WARNING | ERROR
  };

  class Logger {
  public:
    struct Settings {
      static inline uint32_t levelMask     = ALL;
      static inline bool showTimestamp     = true;
      static inline const char *timeFormat = "%d-%m-%Y %T";
    };

    /**
     * Outputs a message to console in the following format:
     * [timestamp] [severity level] [formatted message]
     *
     * @param level The severity level this message should be logged at.
     * @param msg The message to be logged, can contain format specifiers.
     * @param args Additional arguments that can be passed that are used to format the message.
     */
    template <class... Args>
    static void Log(LogType level, const std::string &msg, Args &&...args) {
      // Check if this message should be logged given its level
      if (!(Settings::levelMask & level))
        return;

      // Add timestamp
      if (Settings::showTimestamp)
        std::cout << "[" << GetTimestamp() << "] ";

      // Add level label and formatted message
      std::cout << GetLevelLabel(level) << std::vformat(msg, std::make_format_args(args...))
                << std::endl;
    }

  private:
    Logger() {} // prevent construction of this class

    static inline std::string GetTimestamp() {
      auto timeNow        = std::chrono::system_clock::now();
      std::string timeStr = date::format(Settings::timeFormat, timeNow);

      return timeStr;
    }

    static inline std::string GetLevelLabel(LogType level) {
      std::string levelLabel = "";

      switch (level) {
      case DEBUG:
        levelLabel = "[DEBUG]   ";
        break;
      case INFO:
        levelLabel = "[INFO]    ";
        break;
      case WARNING:
        levelLabel = "[WARNING] ";
        break;
      case ERROR:
        levelLabel = "[ERROR]   ";
        break;
      }

      return levelLabel;
    }
  };
} // namespace TritiumEngine::Utilities