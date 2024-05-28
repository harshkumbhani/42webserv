#include "Lexer.hpp"
#include <exception>
#include <iostream>
#include "EventLogger.hpp"

int main(int argc, char *argv[]) {
  INFO("Web server initialising");
  const char *configfile_path = "./config/default.config";
  if (argc > 2) {
    std::cerr << "Usage: ./webserv [Path to configfile]" << std::endl;
    return 1;
  }
  if (argc == 2)
    configfile_path = argv[1];
  try {
    Lexer tokens(configfile_path);
    std::cout << tokens << std::endl;
    SUCCESS("Tokens created Successfully");
  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
