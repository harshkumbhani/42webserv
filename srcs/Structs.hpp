#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// Global mime type map
extern std::map<std::string, std::string> g_mimeTypes;

enum token {
  HTTP = 0,              // 0
  SERVERBLOCK = 1,       // 1
  KEEPALIVE_TIMEOUT = 2, // default  2
  SEND_TIMEOUT = 3,      // default 3
  LISTEN = 4,            // must 4
  SERVER_NAME = 5,       // must 5
  ROOT = 6,              // needed
  AUTOINDEX = 7,         // default
  INDEX = 8,             // default
  DIR_LISTING = 9,       // default
  CLIENT_BODY_SIZE = 10, // default
  LOCATION = 11,         // needed
  METHODS = 12,
  REDIRECT = 13,
  OPEN_CURLY_BRACKET = 14,
  CLOSED_CURLY_BRACKET = 15,
  SEMICOLON = 16,
  UNKNOWN = 17
};

struct lexer_node {
  token type;
  std::string key;
  std::string value;
};

struct Location {
  std::vector<std::string> methods;
  std::string redirect;
  std::string root;
  std::string index;
  std::string path;
};

struct ServerParser {
  int keepalive_timeout;
  int send_timeout;
  int listen;
  int sockfd;
  std::string server_name;
  std::string root;
  std::string autoindex;
  std::string index;
  std::string directory_listing;
  size_t client_body_size;
  std::vector<Location> location;
};


enum methods { GET = 1, POST = 2, DELETE = 3 };

struct clientState {
  bool flagHeaderRead;
  bool flagBodyRead;
  methods method;
  ssize_t bytesRead;
  ssize_t contentLength;
  time_t startTime;
  std::string bodyString;
  std::vector<char> body;
  std::string readString;
  std::string writeString;
  std::map<std::string, std::string> requestLine;
  std::map<std::string, std::string> header;
  ServerParser serverData;
};

#endif
