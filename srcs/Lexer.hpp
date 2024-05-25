#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum token {
  KEEPALIVE_TIMEOUT, // default
  SEND_TIMEOUT, // default
  LISTEN,
  SERVER_NAME,
  ROOT,
  AUTOINDEX,
  INDEX, // default
  DIR_LISTING,
  CLIENT_BODY_SIZE, // default
  LOCATION,
  METHODS,
  REDIRECT,
  OPEN_CURLY_BRACKET,
  CLOSED_CURLY_BRACKET,
  SEMICOLON,
};

struct lexer_node {
  token type;
  std::string key;
  std::string value;
};

class Lexer {
private:
  std::vector<lexer_node> lexer;
  std::string filename;

public:
  Lexer(const std::string filename);
  ~Lexer();

  token getTokenType(const std::string &type);
  void  tokenize(std::string filename);

  std::vector<lexer_node> getLexer() const;
};

#endif
