#include "Lexer.hpp"
#include "Config.hpp"

int main(int argc, char *argv[]) {
  const char *configfile_path = "./config/default.config";
  if (argc > 2) {
    std::cerr << "Usage: ./webserv [Path to configfile]" << std::endl;
    return 1;
  }
  if (argc == 2)
    configfile_path = argv[1];
  try {
    Lexer tokens(configfile_path);
	Config parser(tokens.getLexer());
    // std::cout << tokens << std::endl;

  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
