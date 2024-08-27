#ifndef LEXER_HPP
#define LEXER_HPP

#include "EventLogger.hpp"
#include "Structs.hpp"

class Lexer {
private:
  std::vector<lexer_node> lexer; // Stores all tokens
  std::string filepath;          // Config file name
  std::string buffer;            // Stores the file stream
  std::map<std::string, token> directive_lookup;

public:
  Lexer(const char *filename);
  ~Lexer(); // Destructor

  void readfileintobuffer();
  token getTokenType(const std::string &type);
  void tokenize(std::string &buffer);
  void createToken(std::vector<std::string>::iterator &begin,
                   std::vector<std::string> &words, lexer_node &node);
  void parseString(const std::string &line);
  std::vector<lexer_node> getLexer() const;
};

std::ostream &operator<<(std::ostream &output, const Lexer &lexer);

#endif
