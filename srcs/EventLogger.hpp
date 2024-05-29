#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";        // SUCCESS
const std::string BLUE = "\033[34m";         // INFO
const std::string YELLOW = "\033[33m";       // WARNING
const std::string RED = "\033[31m";          // ERROR
const std::string ORANGE = "\033[38;5;208m"; // DEBUG

class EventLogger;

#define INFO(message)                                                          \
  {                                                                            \
    std::ostringstream log;                                                    \
    log << message;                                                            \
    EventLogger::log(log.str(), __FILE__, __LINE__, BLUE, EventLogger::INFO);  \
  }

#define DEBUG(message)                                                         \
  {                                                                            \
    std::ostringstream log;                                                    \
    log << message;                                                            \
    EventLogger::log(log.str(), __FILE__, __LINE__, ORANGE,                    \
                     EventLogger::DEBUG);                                      \
  }

#define WARNING(message)                                                       \
  {                                                                            \
    std::ostringstream log;                                                    \
    log << message;                                                            \
    EventLogger::log(log.str(), __FILE__, __LINE__, YELLOW,                    \
                     EventLogger::WARNING);                                    \
  }

#define ERROR(message)                                                         \
  {                                                                            \
    std::ostringstream log;                                                    \
    log << message;                                                            \
    EventLogger::log(log.str(), __FILE__, __LINE__, RED, EventLogger::ERROR);  \
  }

#define SUCCESS(message)                                                       \
  {                                                                            \
    std::ostringstream log;                                                    \
    log << message;                                                            \
    EventLogger::log(log.str(), __FILE__, __LINE__, GREEN,                     \
                     EventLogger::SUCCESS);                                    \
  }

class EventLogger {
private:
  EventLogger();

public:
  enum logLevel { INFO, DEBUG, WARNING, ERROR, SUCCESS };

  ~EventLogger();

  static std::string displayTimeStamp();
  static std::string getLevel(logLevel color);
  static void log(const std::string &message, const char *filename, int lineNumber,
                  const std::string &color, logLevel level);
};

#endif // EVENT_LOGGER_H
