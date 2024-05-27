/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:34:40 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/27 14:56:32 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main() {
	std::vector<lexer_node> configLexa = {
		{SERVER, "", ""},
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
	};

	try {
		Config config;
		if (config.parseConfigurations(configLexa) != 0)
			std::cout << "Error" << std::endl;
			
		std::cout << "Server Configuration:" << std::endl;
		std::cout << "Keepalive Timeout: " << config.serv.keepalive_timeout << "s" << std::endl;
		std::cout << "Send Timeout: " << config.serv.send_timeout << "s" << std::endl;
		std::cout << "Listen: " << config.serv.listen << std::endl;
		std::cout << "Server Name: " << config.serv.server_name << std::endl;
		std::cout << "Root: " << config.serv.root << std::endl;
		std::cout << "Autoindex: " << config.serv.autoindex << std::endl;
		std::cout << "Index: " << config.serv.index << std::endl;
		std::cout << "Directory Listing: " << config.serv.directory_listing << std::endl;
		std::cout << "Client Body Size: " << config.serv.client_body_size << std::endl;

		// for (const Config::Location& loc : server.locations) {
		// 	std::cout << "Location Path: " << loc.path << std::endl;
		// 	std::cout << "Methods: ";
		// 	for (const std::string& method : loc.methods) {
		// 		std::cout << method << " ";
		// 	}
		// 	std::cout << std::endl;
		// 	if (!loc.redirect.empty()) {
		// 		std::cout << "Redirect: " << loc.redirect << std::endl;
		// 	}
		// }
		
	} catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
