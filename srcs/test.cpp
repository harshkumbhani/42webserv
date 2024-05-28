/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:34:40 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/28 14:05:00 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main() {
	std::vector<lexer_node> configLexa = {
		{HTTP, "", ""},
		{OPEN_CURLY_BRACKET, "", ""},
		{SERVERBLOCK, "", ""},
		{OPEN_CURLY_BRACKET, "", ""},
		{KEEPALIVE_TIMEOUT, "", "30s"},
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
		{LOCATION, "/", ""},
		{OPEN_CURLY_BRACKET, "", ""},
		{METHODS, "GET POST", ""},
		{SEMICOLON, "", ""},
		{CLOSED_CURLY_BRACKET, "", ""},
		{CLOSED_CURLY_BRACKET, "", ""},
		{CLOSED_CURLY_BRACKET, "", ""},
	};

	try {
		Config config;
		if (config.parseConfigurations(configLexa) != 0) {
			std::cout << "Error" << std::endl;
			return 1;
		}
		
		// Print all server configurations
		for (const Config::ServerConfig& server : config.servers) {
			std::cout << "Server Configuration:" << std::endl;
			std::cout << "Keepalive Timeout: " << server.keepalive_timeout << "s" << std::endl;
			std::cout << "Send Timeout: " << server.send_timeout << "s" << std::endl;
			std::cout << "Listen: " << server.listen << std::endl;
			std::cout << "Server Name: " << server.server_name << std::endl;
			std::cout << "Root: " << server.root << std::endl;
			std::cout << "Autoindex: " << server.autoindex << std::endl;
			std::cout << "Index: " << server.index << std::endl;
			std::cout << "Directory Listing: " << server.directory_listing << std::endl;
			std::cout << "Client Body Size: " << server.client_body_size << std::endl;

			// Print all location configurations within the server
			for (const Config::Location& loc : server.location) {
				std::cout << "Location Path: " << loc.path << std::endl;
				std::cout << "Methods: ";
				for (const std::string& method : loc.methods) {
					std::cout << method << " ";
				}
				std::cout << std::endl;
				if (!loc.redirect.empty()) {
					std::cout << "Redirect: " << loc.redirect << std::endl;
				}
			}
		}

	} catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
