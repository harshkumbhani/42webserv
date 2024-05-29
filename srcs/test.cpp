/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:34:40 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/29 19:11:00 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "EventLogger.hpp"

int main() {
	try {
		std::vector<lexer_node> lexer = {
			{HTTP, "", ""},
			{OPEN_CURLY_BRACKET, "", ""},
			{SERVERBLOCK, "", ""},
			{OPEN_CURLY_BRACKET, "", ""},
			{KEEPALIVE_TIMEOUT, "", "10s"},
			{SEMICOLON, "", ""},
			{SEND_TIMEOUT, "", "5s"},
			{SEMICOLON, "", ""},
			{LISTEN, "", "8080"},
			{SEMICOLON, "", ""},
			{SERVER_NAME, "", "example.com"},
			{SEMICOLON, "", ""},
			{ROOT, "", "/var/www/html"},
			{SEMICOLON, "", ""},
			{AUTOINDEX, "", "on"},
			{SEMICOLON, "", ""},
			{INDEX, "", "index.html"},
			{SEMICOLON, "", ""},
			{DIR_LISTING, "", "on"},
			{SEMICOLON, "", ""},
			{CLIENT_BODY_SIZE, "", "1000000"},
			{SEMICOLON, "", ""},
			{LOCATION, "", "/images"},
			{OPEN_CURLY_BRACKET, "", ""},
			{METHODS, "", "GET POST"},
			{SEMICOLON, "", ""},
			{REDIRECT, "", "http://example.com"},
			{SEMICOLON, "", ""},
			{ROOT, "", "/var/www/images"},
			{SEMICOLON, "", ""},
			{CLOSED_CURLY_BRACKET, "", ""},
			{CLOSED_CURLY_BRACKET, "", ""},
			{CLOSED_CURLY_BRACKET, "", ""}
		};

		// Initialize the Config object with the lexer nodes
		Config config(lexer);

		// Parse the configurations
		config.parseConfigurations(lexer);

		// If no exception is thrown, print success message
		// std::cout << "Configuration parsed successfully!" << std::endl;

		// Optionally, print out the parsed server configurations
		// for (size_t i = 0; i < config.servers.size(); ++i) {
		// 	std::cout << "Server " << i + 1 << ":" << std::endl;
		// 	std::cout << "  Keepalive Timeout: " << config.servers[i].keepalive_timeout << std::endl;
		// 	std::cout << "  Send Timeout: " << config.servers[i].send_timeout << std::endl;
		// 	std::cout << "  Listen: " << config.servers[i].listen << std::endl;
		// 	std::cout << "  Server Name: " << config.servers[i].server_name << std::endl;
		// 	std::cout << "  Root: " << config.servers[i].root << std::endl;
		// 	std::cout << "  Autoindex: " << config.servers[i].autoindex << std::endl;
		// 	std::cout << "  Index: " << config.servers[i].index << std::endl;
		// 	std::cout << "  Directory Listing: " << config.servers[i].directory_listing << std::endl;
		// 	std::cout << "  Client Body Size: " << config.servers[i].client_body_size << std::endl;
		// 	for (size_t j = 0; j < config.servers[i].location.size(); ++j) {
		// 		std::cout << "  Location " << j + 1 << ":" << std::endl;
		// 		std::cout << "    Path: " << config.servers[i].location[j].path << std::endl;
		// 		std::cout << "    Root: " << config.servers[i].location[j].root << std::endl;
		// 		std::cout << "    Redirect: " << config.servers[i].location[j].redirect << std::endl;
		// 		std::cout << "    Methods: ";
		// 		for (size_t k = 0; k < config.servers[i].location[j].methods.size(); ++k) {
		// 			std::cout << config.servers[i].location[j].methods[k] << " ";
		// 		}
		// 		std::cout << std::endl;
		// 	}
		// }
		// std::cout << "Printed Successfully!" << std::endl;
		

	} catch (const std::runtime_error &e) {
		// Handle any errors that are thrown
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}

