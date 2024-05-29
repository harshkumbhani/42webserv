#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <exception>

#include "EventLogger.hpp"

enum token {
  HTTP,
  SERVERBLOCK,
  KEEPALIVE_TIMEOUT, // default
  SEND_TIMEOUT,      // default
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
  UNKNOWN
};

struct lexer_node {
  token       type;
  std::string key;
  std::string value;
};

class Lexer {
private:
  std::vector<lexer_node>       lexer; // Stores all tokens
  std::string                   filepath; // Config file name
  std::string                   buffer; // Stores the file stream
  std::map<std::string, token>  directive_lookup;

public:
  Lexer(const char *filename);
  ~Lexer(); // Destructor

  void	readfileintobuffer();
  token	getTokenType(const std::string &type);
  void	tokenize(std::string &buffer);
  void	createToken(std::vector<std::string>::iterator &begin, std::vector<std::string> &words, lexer_node &node);
  void	parseString(const std::string &line);
  std::vector<lexer_node> getLexer() const;
};

std::ostream &operator<<(std::ostream &output, const Lexer &lexer);

#endif
