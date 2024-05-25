#include <iostream>
#include <exception>
#include "Lexer.hpp"


int main(int argc, char *argv[]) {
  const char *configfile_path = "./config/default.config";
  if (argc > 2) {
    std::cerr << "Usage: ./webserv [Path to configfile]" << std::endl;
    return 1;
  }
  if (argc == 2)
    configfile_path = argv[1];
  Lexer tokens(configfile_path);
  tokens.readfileintobuffer();
  return 0;
}
