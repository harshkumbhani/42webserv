#include "Lexer.hpp"

Lexer::Lexer(const char *filename) : filepath(filename) {
  directive_lookup["#"] = COMMENT;
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

  readfileintobuffer();
  tokenize(buffer);
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

  configFile.close();
}

token Lexer::getTokenType(const std::string &type) {
  std::map<std::string, token>::iterator it = directive_lookup.find(type);
  if (it != directive_lookup.end())
    return it->second;
  return UNKNOWN;
}

static void trim(std::string &line) {
  size_t  start = line.find_first_not_of(" \n\v\t\r\f");
  if (start == std::string::npos) {
    line = "";
    return;
  }

  size_t  end = line.find_last_not_of(" \n\v\t\r\f") + 1;
  line = line.substr(start, end);
}

void Lexer::tokenize(std::string &buffer) {
  std::istringstream ss(buffer);
  std::string line;

  while (std::getline(ss, line)) {
    trim(line);
    if (line.empty() == true || line[0] == '#')
      continue;
    break;
  }
  if (line != "http {")
    throw std::runtime_error("Error in the config file: Expected 'http'");

  while(std::getline(ss, line)) {
    trim(line);
    lexer_node node;
    std::istringstream lineStream(line);
    std::string word;

    lineStream >> word;
      node.type = getTokenType(word);
      node.key = word;
      lineStream >> word;
      node.value = word;
    lexer.push_back(node);
    lineStream.clear();
  }
}

std::vector<lexer_node> Lexer::getLexer() const { return this->lexer; }

std::ostream &operator<<(std::ostream &output, const Lexer &lexer) {
  std::vector<lexer_node>::iterator it;
  for (it = lexer.getLexer().begin(); it != lexer.getLexer().end(); it++) {
    output << "Type: " << it->type << ", Key: " << it->key
           << ", value: " << it->value << std::endl;
  }
  return output;
}
