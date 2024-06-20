// #include "Lexer.hpp"
// #include "Parser.hpp"
// #include "HttpRequest.hpp"
// #include <exception>
// #include <iostream>
// #include "EventLogger.hpp"

// int main(int argc, char *argv[]) {
// INFO("Web server initialising");
// const char *configfile_path = "./config/default.config";
// if (argc > 2) {
// 	ERROR("Usage: ./webserv or ./webserv [Path to configfile]");
// 	return 1;
// }
// if (argc == 2)
// 	configfile_path = argv[1];
// try {
// 	Lexer tokens(configfile_path);
// 	std::cout << tokens << std::endl;
// 	Parser parser(tokens.getLexer());
// 	std::cout << parser << std::endl;

// } catch (std::runtime_error const &e) {
// 	ERROR(e.what());
// 	return 1;
// }
// return 0;
// }

//TEMPORARY SOCKET FOR TESTING PURPOSES
// int main() {
// 	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
// 	sockaddr_in	serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(8080);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	bind(server_socket, (struct sockaddr *)& serverAddress, sizeof(serverAddress));

// 	listen(server_socket, 5);

// 	int client_socket = accept(server_socket, NULL, NULL);

// 	char buffer[1024] = {0};
// 	recv(client_socket, buffer, sizeof(buffer), 0);

// 	std::cout << "Received message: " << buffer << std::endl;

// 	close(client_socket);
// 	close(server_socket);
// 	return 0;
// }

//TEMPORARY SOCKET FOR TESTING PURPOSES
#include "HttpRequest.hpp"

int main() {
	HttpRequest httpRequest;
	std::string request = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\nContent-Length: 13\r\n\r\nHello, world!";

	httpRequest.request_blocks(request);

	std::cout << "Request Line:\n" << std::endl;
	std::map<std::string, std::string>::const_iterator reqLineIter;
	for (reqLineIter = httpRequest.ReqLine.begin(); reqLineIter != httpRequest.ReqLine.end(); ++reqLineIter) {
		std::cout << reqLineIter->first << ": " << reqLineIter->second << std::endl;
	}

	std::cout << "\nHeaders:\n" << std::endl;
	std::map<std::string, std::string>::const_iterator i;
	for (i = httpRequest._Header.begin(); i != httpRequest._Header.end(); ++i) {
		std::cout << i->first << ": " << i->second << std::endl;
	}
	std::cout << std::endl;

	std::cout << "\nBody:\n" << std::endl;
	std::cout << httpRequest._Body << std::endl;

	return 0;
#include "Parser.hpp"
#include "EventLogger.hpp"
#include "Lexer.hpp"
#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
  INFO("Web server initialising");
  const char *configfile_path = "./config/default.config";
  if (argc > 2) {
    ERROR("Usage: ./webserv or ./webserv [Path to configfile]");
    return 1;
  }
  if (argc == 2)
    configfile_path = argv[1];
  try {
    Lexer tokens(configfile_path);
    std::cout << tokens << std::endl;
    Parser parser(tokens.getLexer());
    std::cout << parser << std::endl;

  } catch (std::runtime_error const &e) {
    ERROR(e.what());
    return 1;
  }
  return 0;
}
