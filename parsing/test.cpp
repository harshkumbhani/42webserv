/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:34:40 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/26 12:07:59 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main() {
	std::vector<lexer_node> configData = {
		{OPEN_CURLY_BRACKET, "", "http"},
		{OPEN_CURLY_BRACKET, "", "server"},
		{KEEPALIVE_TIMEOUT, "", "30"},
		{SEND_TIMEOUT, "", "30"},
		{LISTEN, "", "8080"},
		{SERVER_NAME, "", "example.com"},
		{ROOT, "", "/var/www/html"},
		{AUTOINDEX, "", "on"},
		{INDEX, "", "index.html"},
		{DIR_LISTING, "", "off"},
		{CLIENT_BODY_SIZE, "", "1000000"},
		{OPEN_CURLY_BRACKET, "", "location"},
		{METHODS, "", "GET"},
		{CLOSED_CURLY_BRACKET, "", ""},
		{OPEN_CURLY_BRACKET, "", "location"},
		{METHODS, "", "POST"},
		{CLOSED_CURLY_BRACKET, "", ""},
		{CLOSED_CURLY_BRACKET, "", ""},
		{CLOSED_CURLY_BRACKET, "", ""}
	};

	// Parse configurations
	Config config;
	int result = config.parseConfigurations(configData);

	// Check result
	if (result == 0) {
		std::cout << "KEEPALIVE_TIMEOUT: " << config.serv.keepalive_timeout << std::endl;
		std::cout << "SEND_TIMEOUT: " << config.serv.send_timeout << std::endl;
		std::cout << "LISTEN: " << config.serv.listen << std::endl;
		std::cout << "SERVER_NAME: " << config.serv.server_name << std::endl;
		std::cout << "ROOT: " << config.serv.root << std::endl;
		std::cout << "AUTOINDEX: " << config.serv.autoindex << std::endl;
		std::cout << "INDEX: " << config.serv.index << std::endl;
		std::cout << "DIR_LISTING: " << config.serv.directory_listing << std::endl;
		std::cout << "CLIENT_BODY_SIZE: " << config.serv.client_body_size << std::endl;
		// std::cout << "METHODS: " << config.loc.methods << std::endl;
	
		std::cout << "Configurations parsed successfully." << std::endl;
	} else {
		std::cout << "Error parsing configurations." << std::endl;
	}

	return 0;
}

