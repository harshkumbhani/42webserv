#ifndef STRUCTS_HPP
# define STRUCTS_HPP

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

	void clear() {
		methods.clear();
		redirect.clear();
		root.clear();
		index.clear();
		path.clear();
	}
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

	void clear() {
		location.clear();
		server_name.clear();
		root.clear();
		autoindex.clear();
		index.clear();
		directory_listing.clear();
	}
};


enum methods { GET = 1, POST = 2, DELETE = 3, CGI = 4, DEFAULT = -1};

struct clientState {
	bool flagHeaderRead;
	bool flagBodyRead;
	bool flagPartiallyRead;
	bool isKeepAlive;
	bool closeConnection;
	bool flagFileSizeTooBig;
	bool flagFileStatus;
	bool isForked;
	methods method;
	int	fd[2];
	pid_t	pid;
	ssize_t bytesRead;
	ssize_t contentLength;
	time_t lastEventTime;
	std::string bodyString;
	std::vector<char> body;
	std::string readString;
	std::string writeString;

	std::vector<std::string> requestLine;
	std::map<std::string, std::string> header;
	ServerParser serverData;
	std::string	contentType;
	std::string	boundary;
	std::string	fileName;
	bool killChild;

	void clear() {
	flagHeaderRead = false;
	flagBodyRead = false;
	flagPartiallyRead = false;
	isKeepAlive = false;
	closeConnection = false;
	isForked = false;
	method = DEFAULT; // Or some default method
	pid = -1;
	bytesRead = -1;
	contentLength = 0;
	bodyString.clear();
	body.clear();
	readString.clear();
	writeString.clear();
	requestLine.clear();
	header.clear();
	serverData.clear(); // Assuming ServerParser has a clear method
	contentType.clear();
	boundary.clear();
	fileName.clear();
}
};

#endif