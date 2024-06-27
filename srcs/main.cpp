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
#include "HttpResponse.hpp"

int main() {
	try
	{
		HttpRequest httpRequest;
		//GET
		// std::string request = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\nContent-Length: 13\r\n\r\nHello, world!";
		// std::string request = "GET /x.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\nContent-Length: 13\r\n\r\nHello, world!";

		// POST
		// std::string request = "POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\n\r\nkey1=value1&key2=value2";

		//DELETE
		std::string request = "DELETE /delete-resource.txt HTTP/1.1\r\nHost: www.example.com\r\nContent-Length: 3\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\nAccept: */*\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\n\r\n";

		httpRequest.request_blocks(request);

		std::cout << "======================================================" << std::endl;
		std::cout << "||                      REQUEST                     ||" << std::endl;
		std::cout << "======================================================" << std::endl;
		std::cout << "Request Line:\n" << std::endl;
		std::map<std::string, std::string>::const_iterator reqLineIter;
		for (reqLineIter = httpRequest._ReqLine.begin(); reqLineIter != httpRequest._ReqLine.end(); ++reqLineIter) {
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

		HttpResponse	HttpResponse;
		HttpResponse.respond(httpRequest);
		std::cout << "======================================================" << std::endl;
		std::cout << "||                     RESPONSE                     ||" << std::endl;
		std::cout << "======================================================" << std::endl;
		std::cout << "Response:\n" << std::endl;
		std::cout << HttpResponse._Response << std::endl;
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
// #include "Parser.hpp"
// #include "EventLogger.hpp"
// #include "Lexer.hpp"
// #include <exception>
// #include <iostream>

// int main(int argc, char *argv[]) {
//   INFO("Web server initialising");
//   const char *configfile_path = "./config/default.config";
//   if (argc > 2) {
//     ERROR("Usage: ./webserv or ./webserv [Path to configfile]");
//     return 1;
//   }
//   if (argc == 2)
//     configfile_path = argv[1];
//   try {
//     Lexer tokens(configfile_path);
//     std::cout << tokens << std::endl;
//     Parser parser(tokens.getLexer());
//     std::cout << parser << std::endl;

//   } catch (std::runtime_error const &e) {
//     ERROR(e.what());
//     return 1;
//   }
//   return 0;
}
