#include "Lexer.hpp"

Lexer::Lexer(const char *filename) : filepath(filename) {
  directive_lookup = {
    {"server", SERVERBLOCK},
    {"keepalive_timeout", KEEPALIVE_TIMEOUT},
    {"send_timeout", SEND_TIMEOUT},
    {"listen", LISTEN},
    {"server_name", SERVER_NAME},
    {"root", ROOT},
    {"autoindex", AUTOINDEX},
    {"index", INDEX},
    {"directory_listing", DIR_LISTING},
    {"client_body_size", CLIENT_BODY_SIZE},
    {"location", LOCATION},
    {"methods", METHODS},
    {"redirect", REDIRECT},
    {"{", OPEN_CURLY_BRACKET},
    {"}", CLOSED_CURLY_BRACKET},
    {";", SEMICOLON}
  };
}

Lexer::~Lexer() {}

void Lexer::readfileintobuffer() {
  std::ifstream configFile(filepath.c_str());
  if (configFile.fail() == true) {
    std::cerr << "Error opening the file" << std::endl;
    return;
  }

  buffer.assign(std::istreambuf_iterator<char>(configFile),
                std::istreambuf_iterator<char>());
}

token Lexer::getTokenType(const std::string &type) {
  std::map<std::string, token>::iterator it =  directive_lookup.find(type);
  if (it != directive_lookup.end())
    return it->second;
  return UNKNOWN;
}
