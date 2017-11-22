#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

class Log {
 public:
  /**
   * Log levels for the application.
   */
  enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
  };

  static Log& getInstance();

  /**
   * Set log level
   * @method setName
   * @param LogLevel Log level to set.
   */
  void setLevel(LogLevel);

  /**
   * Log a debug info to the default output stream.
   * @method logInfo
   * @param  std::string The message to log.
   */
  void logDebug(std::string);

  /**
   * Log an info to the default output stream.
   * @method logInfo
   * @param  std::string The message to log.
   */
  void logInfo(std::string);

  /**
   * Log a warning to the default error stream.
   * @method logWarning
   * @param  std::string The message to log.
   */
  void logWarning(std::string);

  /**
   * Log an error to the default error stream.
   * @method logError
   * @param  std::string The message to log.
   */
  void logError(std::string);

  /**
   * Log a simple message(without prefix) to the default output stream.
   * @method logSimple
   * @param  std::string The message to log.
   */
  void logSimple(std::string);

 private:
   /* Constructor and destructor */
  Log() {};
  virtual ~Log() {};

  /* Log level */
  LogLevel logLevel = LOG_DEBUG;
};

#endif /* end of include guard: LOG_H */
