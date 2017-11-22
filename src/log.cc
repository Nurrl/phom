#include "log.h"

Log& Log::getInstance() {
  static Log _Log;

  return _Log;
}

void Log::setLevel(LogLevel logLevel) {
  this->logLevel = logLevel;
}

void Log::logDebug(std::string logStr) {
  if(this->logLevel == LogLevel::LOG_DEBUG)
    std::cout << ":d: " << logStr << std::endl;
}

void Log::logInfo(std::string logStr) {
  if(this->logLevel == LogLevel::LOG_DEBUG
  || this->logLevel == LogLevel::LOG_INFO)
    std::cout << ":i: " << logStr << std::endl;
}

void Log::logWarning(std::string logStr) {
  if(this->logLevel == LogLevel::LOG_DEBUG
  || this->logLevel == LogLevel::LOG_INFO
  || this->logLevel == LogLevel::LOG_WARN)
    std::cerr << ":!: " << logStr << std::endl;
}

void Log::logError(std::string logStr) {
  if(this->logLevel == LogLevel::LOG_DEBUG
  || this->logLevel == LogLevel::LOG_INFO
  || this->logLevel == LogLevel::LOG_WARN
  || this->logLevel == LogLevel::LOG_ERROR)
    std::cerr << ":x: " << logStr << std::endl;
}

void Log::logSimple(std::string logStr) {
  std::cout << logStr << std::endl;
}
