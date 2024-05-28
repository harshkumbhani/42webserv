#include "EventLogger.hpp"

EventLogger::EventLogger() {}

EventLogger::~EventLogger() {}

std::string EventLogger::getLevel(EventLogger::logLevel color) {
  std::string logLevel;

  switch (color) {
  case INFO:
    return "INFO";
  case DEBUG:
    return "DEBUG";
  case WARNING:
    return "WARNING";
  case ERROR:
    return "ERROR";
  case SUCCESS:
    return "SUCCESS";
  default:
    break;
  }
  return "reset";
}

void EventLogger::log(const std::string &message, const char *filename,
                      int lineNumber, const std::string &color,
                      logLevel level) {
  std::cout << color << "[ " << getLevel(level) << " ] "
            << "[ " << filename << ", LN: " << lineNumber << " ] : " << message
            << RESET << std::endl;
}
