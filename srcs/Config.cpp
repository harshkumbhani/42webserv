/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/29 19:31:15 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(std::vector<lexer_node> lexer) : lexer(lexer) {
	parseConfigurations(this->lexer);
}

Config::~Config() {}

std::vector<ServerConfig> Config::getParser() const {
	return this->servers;
}

inline void Config::error(const std::string& str){
	throw std::runtime_error(str);
}

//-->server features
void	Config::parseKeepaliveTimeout(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	int seconds;
	std::string measure;
	std::istringstream iss(it->value);
	if (!(iss >> seconds)) {
		error("Invalid integer for keepalive_timeout!");
	} else {
		if (iss >> measure) {
			if (measure != "s") {
				error("Wrong measurement! Should be seconds(s)(keepalive timeout)");
			}
		} else if (!iss.eof()) {
			error("Invalid format for keepalive_timeout!");
		}
	}
	server.keepalive_timeout = seconds;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Keepalive timeout is missing a semi-colon!");
}

void	Config::parseSendTimeout(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	int seconds;
	std::string measure;
	std::istringstream iss(it->value);
	if (!(iss >> seconds)) {
		error("Invalid integer for Send timeout!");
	} else {
		if (iss >> measure) {
			if (measure != "s") {
				error("Wrong measurement! Should be seconds(s) (Send timeout)");
			}
		} else if (!iss.eof()) {
			error("Invalid format for Send timeout!");
		}
	}
	server.send_timeout = seconds;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Send timeout is missing semi-colon!");
}

void	Config::parseListen(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	int port;
	std::istringstream iss(it->value);
	if (!(iss >> port)) {
		error("Invalid integer for listen!");
	} else if (!iss.eof()) {
		error("Invalid format for listen!");
	}
	server.listen = port;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Listen dir is missing semicolon!");
}

void	Config::parseServerName(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.server_name = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Servername dir is missing semi-colon");
}

void	Config::parseRoot(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.root = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Root dir is missing semi-colon!");
}

void	Config::parseAutoindex(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.autoindex = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Autoindex dir is missing semi colon!");
}

void	Config::parseIndex(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.index = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Index dir is missing semi colon!");
}

void	Config::parseDirListing(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.directory_listing = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Dir Listing is missing semi colon!");
}

void	Config::parseClientBodySize(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	size_t size;
	std::istringstream iss(it->value);
	if (!(iss >> size)) {
		error("Invalid Num for size!");
	} else if (!iss.eof()) {
		error("Invalid format for size!");
	}
	server.client_body_size = size;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Client Body Size is missing semi colon!");
}

void	Config::parseMethods(std::vector<lexer_node>::iterator &it, Location loc) {
	std::istringstream iss(it->value);
	std::string	methos;
	while (iss >> methos) {
		DEBUG(methos);
		loc.methods.push_back(methos);
	}
	
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Method dir is missing semi colon!");
}

void	Config::parseRedirect(std::vector<lexer_node>::iterator &it, Location loc) {
	loc.redirect = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Method dir is missing semi colon!");
}

void	Config::parseLocationRoot(std::vector<lexer_node>::iterator &it, Location loc) {
	loc.root = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
		error("Location root is missing semi colon!");
}

void	Config::parseLocationBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets, ServerConfig &server) {
	Location	loc;
	loc.path = it->value;
	if ((it + 1) != lexer.end() && (it + 1)->type != OPEN_CURLY_BRACKET)
		error("Open Curly Bracket missing at the Location Block!");
	countCurlBrackets++;
	if ((it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
		error("An empty Location Block!");
	DEBUG(countCurlBrackets);
	while (it != lexer.end()) {
		if (countCurlBrackets == 2)
			break ;
		switch (it->type)
		{
		case (METHODS):
			parseMethods(it, loc);
			break;
		case (REDIRECT):
			parseRedirect(it, loc);
			break;
		case (ROOT):
			parseLocationRoot(it, loc);
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
		++it;
	}
	server.location.push_back(loc);
}

void	Config::parseServerBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets) {
	ServerConfig	server;
	if ((it + 1) != lexer.end() && (it + 1)->type != OPEN_CURLY_BRACKET)
		error("Open Curly Bracket missing at the Server Block!");
	if ((it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
		error("An empty Server Block!");
	it++;
	while (it != lexer.end()) {
		switch (it->type)
		{
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
			break ;
		it++;
	}
	servers.push_back(server);
}

int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
	int countCurlBrackets = 0;
	INFO("Parsing initiated");
	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
		switch (it->type)
		{
		case (HTTP):
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
	}
	countCurlBrackets--;
	if (countCurlBrackets != 0) {
		std::cout << countCurlBrackets << std::endl;
		error("Curr brackets error");
	}
	INFO("Parsing completed");
	return (1);
}

std::ostream &operator<<(std::ostream &output, const Config &parser) {
  const std::vector<ServerConfig> &nodes = parser.getParser();
  std::vector<ServerConfig>::const_iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++) {
    output << "keepalive_timeout: " << it->keepalive_timeout
			<< "\nsend timeout: " << it->send_timeout
			<< "\nlisten: " << it->listen
			<< "\nserver_name: " << it->server_name
			<< "\nroot: " << it->root
			<< "\nautoindex: " << it->autoindex
			<< "\nindex: " << it->index
			<< "\ndirectory listing: " << it->directory_listing
			<< "\nclient body size: " << it->client_body_size
			<< std::endl;
  }
  return output;
}
