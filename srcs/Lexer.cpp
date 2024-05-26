#include "Lexer.hpp"

Lexer::Lexer(const char *filename) : filepath(filename) {
  directive_lookup["http"] = HTTP;
  directive_lookup["server"] = SERVERBLOCK;
  directive_lookup["keepalive_timeout"] = KEEPALIVE_TIMEOUT;
  directive_lookup["send_timeout"] = SEND_TIMEOUT;
  directive_lookup["listen"] = LISTEN;
  directive_lookup["server_name"] = SERVER_NAME;
  directive_lookup["root"] = ROOT;
  directive_lookup["autoindex"] = AUTOINDEX;
  directive_lookup["index"] = INDEX;
  directive_lookup["directory_listing"] = DIR_LISTING;
  directive_lookup["client_body_size"] = CLIENT_BODY_SIZE;
  directive_lookup["location"] = LOCATION;
  directive_lookup["methods"] = METHODS;
  directive_lookup["redirect"] = REDIRECT;
  directive_lookup["{"] = OPEN_CURLY_BRACKET;
  directive_lookup["}"] = CLOSED_CURLY_BRACKET;
  directive_lookup[";"] = SEMICOLON;
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

  std::istringstream ss(buffer);
  std::string word;

  while (ss >> word)
    std::cout << word << std::endl;
}

token Lexer::getTokenType(const std::string &type) {
  std::map<std::string, token>::iterator it =  directive_lookup.find(type);
  if (it != directive_lookup.end())
    return it->second;
  return UNKNOWN;
}
