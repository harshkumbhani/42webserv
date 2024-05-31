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
#include <algorithm>
#include <set>

#include "EventLogger.hpp"

enum token {
  HTTP = 0, // 0
  SERVERBLOCK = 1, // 1
  KEEPALIVE_TIMEOUT = 2, // default  2
  SEND_TIMEOUT = 3,      // default 3
  LISTEN = 4, // must 4
  SERVER_NAME = 5, // must 5
  ROOT = 6, // needed
  AUTOINDEX = 7, // default
  INDEX = 8, // default
  DIR_LISTING = 9, // default
  CLIENT_BODY_SIZE = 10, // default
  LOCATION = 11, // needed
  METHODS = 12,
  REDIRECT = 13,
  OPEN_CURLY_BRACKET = 14,
  CLOSED_CURLY_BRACKET = 15,
  SEMICOLON = 16,
  UNKNOWN = 17
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
