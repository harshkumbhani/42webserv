#include "EventLogger.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "SocketManager.hpp"
#include "Utils.hpp"
#include <exception>

int main(int argc, char *argv[]) {
  INFO("Web server initialising");
  const char *configfile_path = "./config/default.config";
  if (argc > 2) {
    ERROR("Usage: ./webserv or ./webserv [Path to configfile]");
    return 1;
  }
  if (argc == 2)
    configfile_path = argv[1];
  try {
    Lexer tokens(configfile_path);
    Parser parser(tokens.getLexer());
    // DEBUG("Number of servers: " << parser.getParser().size());
    SocketManager sockets(parser.getParser());
    // std::cout << sockets.getServers() << std::endl;
  } catch (std::runtime_error const &e) {
    ERROR(e.what());
    return 1;
  }

  return 0;
}
