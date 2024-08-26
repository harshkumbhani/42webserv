#include "Parser.hpp"

Parser::Parser(std::vector<lexer_node> lexer) : lexer(lexer) {
  parseConfigurations(this->lexer);
  parseMimeTypes(mimeTypeFilePath);
}

Parser::~Parser() {}

std::vector<ServerParser> Parser::getParser() const { return this->servers; }

//-->Server features
void Parser::parseKeepaliveTimeout(std::vector<lexer_node>::iterator &it,
                                   ServerParser &server) {
  int seconds;
  std::string measure;
  std::istringstream iss(it->value);
  if (!(iss >> seconds)) {
    throw std::runtime_error("Invalid integer for keepalive_timeout!");
  } else {
    if (iss >> measure) {
      if (measure != "s") {
        throw std::runtime_error(
            "Wrong measurement! Should be seconds(s)(keepalive timeout)");
      }
    } else if (!iss.eof()) {
      throw std::runtime_error("Invalid format for keepalive_timeout!");
    }
  }
  server.keepalive_timeout = seconds;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Keepalive timeout is missing a semi-colon!");
}

void Parser::parseSendTimeout(std::vector<lexer_node>::iterator &it,
                              ServerParser &server) {
  int seconds;
  std::string measure;
  std::istringstream iss(it->value);
  if (!(iss >> seconds)) {
    throw std::runtime_error("Invalid integer for Send timeout!");
  } else {
    if (iss >> measure) {
      if (measure != "s") {
        throw std::runtime_error(
            "Wrong measurement! Should be seconds(s) (Send timeout)");
      }
    } else if (!iss.eof()) {
      throw std::runtime_error("Invalid format for Send timeout!");
    }
  }
  server.send_timeout = seconds;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Send timeout is missing semi-colon!");
}

void Parser::parseListen(std::vector<lexer_node>::iterator &it,
                         ServerParser &server) {
  int port;
  std::istringstream iss(it->value);
  if (!(iss >> port)) {
    throw std::runtime_error("Invalid integer for listen!");
  } else if (!iss.eof()) {
    throw std::runtime_error("Invalid format for listen!");
  }
  server.listen = port;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Listen dir is missing semicolon!");
}

void Parser::parseServerName(std::vector<lexer_node>::iterator &it,
                             ServerParser &server) {
  server.server_name = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Servername dir is missing semi-colon");
}

void Parser::parseRoot(std::vector<lexer_node>::iterator &it,
                       ServerParser &server) {
  server.root = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Root dir is missing semi-colon!");
}

void Parser::parseAutoindex(std::vector<lexer_node>::iterator &it,
                            ServerParser &server) {
  server.autoindex = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Autoindex dir is missing semi colon!");
}

void Parser::parseIndex(std::vector<lexer_node>::iterator &it,
                        ServerParser &server) {
  server.index = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Index dir is missing semi colon!");
}

void Parser::parseDirListing(std::vector<lexer_node>::iterator &it,
                             ServerParser &server) {
  server.directory_listing = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Dir Listing is missing semi colon!");
}

void Parser::parseClientBodySize(std::vector<lexer_node>::iterator &it,
                                 ServerParser &server) {
  size_t size;
  std::istringstream iss(it->value);
  if (!(iss >> size)) {
    throw std::runtime_error("Invalid Num for size!");
  } else if (!iss.eof()) {
    throw std::runtime_error("Invalid format for size!");
  }
  server.client_body_size = size;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Client Body Size is missing semi colon!");
}

//-->Location features
void Parser::parseMethods(std::vector<lexer_node>::iterator &it,
                          Location &loc) {
  std::istringstream iss(it->value);
  std::string methos;
  while (iss >> methos) {
    loc.methods.push_back(methos);
  }

  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Method dir is missing semi colon!");
}

void Parser::parseRedirect(std::vector<lexer_node>::iterator &it,
                           Location &loc) {
  loc.redirect = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Method dir is missing semi colon!");
}

void Parser::parseLocationRoot(std::vector<lexer_node>::iterator &it,
                               Location &loc) {
  loc.root = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Location root is missing semi colon!");
}

void Parser::parseLocationIndex(std::vector<lexer_node>::iterator &it,
                                Location &loc) {
  loc.index = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Index dir is missing semi colon!");
}

void Parser::finaliseLocation(Location &loc, ServerParser &server) {
  if (loc.root == "")
    loc.root = server.root;
  if (loc.index == "")
    loc.index = server.index;
}

void Parser::parseLocationBlock(std::vector<lexer_node>::iterator &it,
                                int &countCurlBrackets, ServerParser &server) {
  Location loc = (struct Location){};
  if (it->value == "")
    throw std::runtime_error("Missing Location Path");
  loc.path = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != OPEN_CURLY_BRACKET)
    throw std::runtime_error(
        "Open Curly Bracket missing at the Location Block!");
  it++;
  if ((it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
    throw std::runtime_error("An empty Location Block!");
  while (it != lexer.end()) {
    switch (it->type) {
    case (METHODS):
      parseMethods(it, loc);
      break;
    case (REDIRECT):
      parseRedirect(it, loc);
      break;
    case (ROOT):
      parseLocationRoot(it, loc);
      break;
    case (INDEX):
      parseLocationIndex(it, loc);
      break;
    case (SEMICOLON):
      break;
    case (OPEN_CURLY_BRACKET):
      countCurlBrackets++;
      break;
    case (CLOSED_CURLY_BRACKET):
      countCurlBrackets--;
      break;
    default:
      break;
    }
    if (countCurlBrackets == 2)
      break;
    ++it;
  }
  server.location.push_back(loc);
}

void Parser::finaliseServer(ServerParser &server) {
  if (server.keepalive_timeout == 0)
    server.keepalive_timeout = 60;
  if (server.send_timeout == 0)
    server.send_timeout = 100;
  if (server.autoindex == "")
    server.autoindex = "off";
  if (server.listen == 0)
    throw std::runtime_error(
        "Listen directive not defined inside a server block");
  if (server.server_name == "")
    throw std::runtime_error(
        "Server Name directive not defined inside a server block");
  if (server.root == "")
    throw std::runtime_error(
        "Root directive not defined inside a server block");
  if ((server.index == "") && (server.autoindex == "off"))
    throw std::runtime_error(
        "Index directive not defined inside a server block");
  else if ((server.index == "") && (server.autoindex == "on"))
    server.index = "index.html";
  if (server.directory_listing == "")
    server.directory_listing = "no";
  if (server.client_body_size == 0)
    server.client_body_size = 5000000;
  std::vector<Location>::iterator it;
  for (it = server.location.begin(); it != server.location.end(); ++it)
    finaliseLocation(*it, server);
}

void Parser::parseServerBlock(std::vector<lexer_node>::iterator &it,
                              int &countCurlBrackets) {
  ServerParser server = (struct ServerParser){};
  server.sockfd = -1;
  if ((it + 1) != lexer.end() && (it + 1)->type != OPEN_CURLY_BRACKET)
    throw std::runtime_error("Open Curly Bracket missing at the Server Block!");
  if ((it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
    throw std::runtime_error("An empty Server Block!");
  it++;
  while (it != lexer.end()) {
    switch (it->type) {
    case (KEEPALIVE_TIMEOUT):
      parseKeepaliveTimeout(it, server);
      break;
    case (SEMICOLON):
      break;
    case (SEND_TIMEOUT):
      parseSendTimeout(it, server);
      break;
    case (LISTEN):
      parseListen(it, server);
      break;
    case (SERVER_NAME):
      parseServerName(it, server);
      break;
    case (ROOT):
      parseRoot(it, server);
      break;
    case (AUTOINDEX):
      parseAutoindex(it, server);
      break;
    case (INDEX):
      parseIndex(it, server);
      break;
    case (DIR_LISTING):
      parseDirListing(it, server);
      break;
    case (CLIENT_BODY_SIZE):
      parseClientBodySize(it, server);
      break;
    case (LOCATION):
      parseLocationBlock(it, countCurlBrackets, server);
      break;
    case (OPEN_CURLY_BRACKET):
      countCurlBrackets++;
      break;
    case (CLOSED_CURLY_BRACKET):
      countCurlBrackets--;
      break;
    default:
      break;
    }
    if (countCurlBrackets == 1)
      break;
    it++;
  }
  finaliseServer(server);
  servers.push_back(server);
}

void Parser::parseConfigurations(std::vector<lexer_node> &lexa) {
  int countCurlBrackets = 0;
  INFO("Parsing initiated");
  if (lexa.empty() == true)
    throw std::runtime_error("Empty config file");
  if (lexa.begin()->type != HTTP)
    throw std::runtime_error("No http block defined");
  std::vector<lexer_node>::iterator it;
  for (it = lexa.begin(); it != lexa.end(); ++it) {
    switch (it->type) {
    case (HTTP):
      if (((it + 1) != lexa.end()) && (it + 1)->type != OPEN_CURLY_BRACKET)
        throw std::runtime_error("Expected token } near HTTP");
      break;
    case (SERVERBLOCK):
      parseServerBlock(it, countCurlBrackets);
      break;
    case (OPEN_CURLY_BRACKET):
      countCurlBrackets++;
      break;
    case (CLOSED_CURLY_BRACKET):
      countCurlBrackets--;
      break;
    default:
      break;
    }
    if (it == lexa.end()) {
      break;
    }
  }
  if (countCurlBrackets != 0) {
    throw std::runtime_error("Unbalanced curly brackets");
  }
  if (servers.empty() == true)
    throw std::runtime_error("Did you magically make server block disapper?");
  SUCCESS("Parsing completed");
}

std::ostream &operator<<(std::ostream &output, const Location &location) {
  output << "\nLocation block start: \n";
  output << "redirect: " << location.redirect << "\nroot: " << location.root
         << "\npath: " << location.path << "\nindex: " << location.index
         << "\nmethods: ";

  std::vector<std::string>::const_iterator it;
  for (it = location.methods.begin(); it != location.methods.end(); it++) {
    output << *it << ", ";
  }
  output << "\n";
  return output;
}

std::ostream &operator<<(std::ostream &output, const std::vector<ServerParser> &nodes) {
  // const std::vector<ServerParser> &nodes = parser.getParser();
  std::vector<ServerParser>::const_iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++) {
    INFO("Server printing started\n");
    output << "keepalive_timeout: " << it->keepalive_timeout
           << "\nsockfd: " << it->sockfd
           << "\nsend timeout: " << it->send_timeout
           << "\nlisten: " << it->listen << "\nserver_name: " << it->server_name
           << "\nroot: " << it->root << "\nautoindex: " << it->autoindex
           << "\nindex: " << it->index
           << "\ndirectory listing: " << it->directory_listing
           << "\nclient body size: " << it->client_body_size
           << "\nLocation: \n";
    std::vector<Location>::const_iterator itl;
    for (itl = it->location.begin(); itl != it->location.end(); itl++) {
      output << *itl;
    }
    INFO("Server printing ended\n");
  }
  return output;
}

std::ostream &operator<<(std::ostream &output, const ServerParser &nodes) {
  // const std::vector<ServerParser> &nodes = parser.getParser();
  // std::vector<ServerParser>::const_iterator it;
  // for (it = nodes.begin(); it != nodes.end(); it++) {
    INFO("Server printing started\n");
    output << "keepalive_timeout: " << nodes.keepalive_timeout
           << "\nsockfd: " << nodes.sockfd
           << "\nsend timeout: " << nodes.send_timeout
           << "\nlisten: " << nodes.listen << "\nserver_name: " << nodes.server_name
           << "\nroot: " << nodes.root << "\nautoindex: " << nodes.autoindex
           << "\nindex: " << nodes.index
           << "\ndirectory listing: " << nodes.directory_listing
           << "\nclient body size: " << nodes.client_body_size
           << "\nLocation: \n";
    std::vector<Location>::const_iterator itl;
    for (itl = nodes.location.begin(); itl != nodes.location.end(); itl++) {
      output << *itl;
    }
    INFO("Server printing ended\n");
  // }
  return output;
}
