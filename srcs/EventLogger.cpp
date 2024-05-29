#include "EventLogger.hpp"

EventLogger::EventLogger() {}

EventLogger::~EventLogger() {}

std::string EventLogger::getLevel(EventLogger::logLevel color) {
  std::string logLevel;

  switch (color) {
  case INFO:
    return "ℹ️  INFO";
  case DEBUG:
    return "🐞 DEBUG";
  case WARNING:
    return "⚠️  WARNING";
  case ERROR:
    return "🔴 ERROR";
  case SUCCESS:
    return "✅ SUCCESS";
  default:
    break;
  }
  return "reset";
}

std::string EventLogger::displayTimeStamp(void) {
  std::time_t currentTime = std::time(NULL);
  std::tm *now = std::localtime(&currentTime);

  char fTime[13];
  std::strftime(fTime, sizeof(fTime), " [%H:%M:%S] ", now);
  return std::string(fTime);
}

void EventLogger::log(const std::string &message, const char *filename,
                      int lineNumber, const std::string &color,
                      logLevel level) {
  std::cout << color << "[ " << getLevel(level) << " ]"
            << EventLogger::displayTimeStamp()
            << "[ " << filename << ":" << lineNumber << " ] : " << message
            << RESET << std::endl;
}
