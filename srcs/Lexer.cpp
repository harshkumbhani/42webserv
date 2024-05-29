#include "Lexer.hpp"

Lexer::Lexer(const char *filename) : filepath(filename) {
  INFO("Tokenising " + std::string(filename) + " file");
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
  SUCCESS("Tokenised " + std::string(filename) + " file");
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
  size_t start = line.find_first_not_of(" \n\v\t\r\f");
  if (start == std::string::npos) {
    line = "";
    return;
  }

  size_t end = line.find_last_not_of(" \n\v\t\r\f") + 1;
  line = line.substr(start, end);
}

void Lexer::createToken(std::vector<std::string>::iterator &begin,
                        std::vector<std::string> &words, lexer_node &node) {
  node.key = *begin;
  ++begin;
  if (words.size() == 3 && begin != words.end())
    node.value = *begin;
  else {
    throw std::runtime_error("Syntax Error near: " + node.key);
  }
}

void Lexer::parseString(const std::string &line) {
  std::vector<std::string> words;
  std::istringstream ss(line);
  std::string directive;

  while (ss >> directive) {
    if (directive[0] == '#')
      break;
    if (directive.find(';') != std::string::npos) {
      directive = directive.substr(0, directive.find_first_of(";"));
      if (directive.empty() != true)
        words.push_back(directive);
      words.push_back(";");
    } else {
      words.push_back(directive);
    }
  }

  std::vector<std::string>::iterator it;
  // for(it = words.begin(); it != words.end(); it++)
  //   std::cout << *it << std::endl;
  size_t counter = -1;
  for (it = words.begin(); it != words.end(); ++it) {
    lexer_node node;
    node.type = getTokenType(*it);

    switch (node.type) {
      case HTTP:
      case SERVERBLOCK:
      case OPEN_CURLY_BRACKET:
      case CLOSED_CURLY_BRACKET:
      case SEMICOLON:
        node.value = *it;
        break;
      case KEEPALIVE_TIMEOUT:
      case SEND_TIMEOUT:
      case LISTEN:
      case SERVER_NAME:
      case ROOT:
      case AUTOINDEX:
      case INDEX:
      case DIR_LISTING:
      case CLIENT_BODY_SIZE:
      case REDIRECT:
        createToken(it, words, node);
        break;
      case LOCATION:
        node.key = *it;
        ++it;
        if (it != words.end())
          node.value = *it;
        break;
      case METHODS:
        node.key = *it;
        while (++it != words.end() && *it != ";") {
          node.value += (node.value.empty() ? "" : " ") + *it;
          counter++;
        }
        if (it != words.end() && *it == ";")
          --it;
        if (counter >= 3)
          throw std::runtime_error("Too many arguments: " + node.key);
        break;
      case UNKNOWN:
        throw std::runtime_error("Unkown token type " + *it);
      default:
        break;
    }
    lexer.push_back(node);
  }
}

void Lexer::tokenize(std::string &buffer) {
  std::istringstream ss(buffer);
  std::string line;

  while (std::getline(ss, line)) {
    trim(line);
    if (line.empty() == true || line[0] == '#')
      continue;
    parseString(line);
  }
}

std::vector<lexer_node> Lexer::getLexer() const { return this->lexer; }

std::ostream &operator<<(std::ostream &output, const Lexer &lexer) {
  const std::vector<lexer_node> &nodes = lexer.getLexer();
  std::vector<lexer_node>::const_iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++) {
    output << "Type: " << it->type << ", Key: " << it->key
           << ", value: " << it->value << "\n";
  }
  return output;
}
