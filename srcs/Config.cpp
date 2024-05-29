/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/29 02:04:52 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(std::vector<lexer_node> lexer) : lexer(lexer) {}

Config::~Config() {}

inline void Config::error(const std::string& str){
	throw std::runtime_error(str);
}

void	Config::parseHttpBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets) {
	if ((it + 1) != lexer.end() && it->type != OPEN_CURLY_BRACKET)
		error("Open Curly Bracket missing at the HTTP Directive Block!");
	countCurlBrackets++;
	if ((it + 1)->type == OPEN_CURLY_BRACKET && (it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
		error("An empty Http Block!");
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
	it++;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
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
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Listen dir is missing semicolon!");
}

void	Config::parseServerName(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.server_name = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Servername dir is missing semi-colon");
}

void	Config::parseRoot(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.root = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Root dir is missing semi-colon!");
}

void	Config::parseAutoindex(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.autoindex = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Autoindex dir is missing semi colon!");
}

void	Config::parseIndex(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.index = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Index dir is missing semi colon!");
}

void	Config::parseDirListing(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	server.directory_listing = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Dir Listing is missing semi colon!");
}

void	Config::parseClientBodySize(std::vector<lexer_node>::iterator &it, ServerConfig &server) {
	long long size;
	std::istringstream iss(it->value);
	if (!(iss >> size)) {
		error("Invalid Num for size!");
	} else if (!iss.eof()) {
		error("Invalid format for size!");
	}
	server.client_body_size = size;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Client Body Size is missing semi colon!");
}

void	Config::parseMethods(std::vector<lexer_node>::iterator &it, Location loc) {
	std::istringstream iss(it->key);
	std::string	methos;
	while (iss >> methos)
		loc.methods.push_back(methos);
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Method dir is missing semi colon!");
}

void	Config::parseRedirect(std::vector<lexer_node>::iterator &it, Location loc) {
	loc.redirect = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Method dir is missing semi colon!");
}

void	Config::parseLocationRoot(std::vector<lexer_node>::iterator &it, Location loc) {
	loc.root = it->value;
	if ((it + 1) != lexer.end() && it->type != SEMICOLON)
		error("Location root is missing semi colon!");
}

void	Config::parseLocationBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets, ServerConfig &server) {
	Location	loc;
	loc.path = it->key;
	if ((it + 1) != lexer.end() && (it++)->type == OPEN_CURLY_BRACKET)
		countCurlBrackets++;
	if ((it + 1)->type == OPEN_CURLY_BRACKET && (it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
		error("An empty Location Block!");
	for (it; it != lexer.end(); ++it) {
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
		case (CLOSED_CURLY_BRACKET):
			countCurlBrackets--;
			break;
		default:
			break;
		}
	}
	server.location.push_back(loc);
}

void	Config::parseServerBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets) {
	ServerConfig	server;
	if ((it + 1) != lexer.end() && (it++)->type == OPEN_CURLY_BRACKET)
		countCurlBrackets++;
	if ((it + 1)->type == OPEN_CURLY_BRACKET && (it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
		error("An empty Server Block!");
	for (it; it != lexer.end(); ++it) {
		if (countCurlBrackets == 1)
			break ;
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
			parseListen(it, server);
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
	}
	// if (it->type != OPEN_CURLY_BRACKET)
	// 	error("Open Curly Bracket missing at the Server Directive Block!");
	// countCurlBrackets++;
	// if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
	// 	error("An empty Server Block!");
}

int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
	int countCurlBrackets = 0;
	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
		switch (it->type)
		{
		case (HTTP):
			parseHttpBlock(it, countCurlBrackets);
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
	if (countCurlBrackets != 0)
		error("Curr brackets error");
}

// Check http followed by {
// If there is a server block, I will call a function
// If inside the server function there is location-> call function



// int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
// 	int countCurlBrackets = 0;
// 	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
// 		Location	location;
// 		switch (it->type)
// 		{
// 		case(HTTP):
// 			it++;
// 			if (it->type != OPEN_CURLY_BRACKET)
// 				error("Open Curly Bracket missing at the HTTP Directive Block!");
// 			countCurlBrackets++;
// 			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
// 				error("An empty Http Block!");
// 			break;
// 		case(SERVERBLOCK):
// 			it++;
// 			if (it->type != OPEN_CURLY_BRACKET)
// 				error("Open Curly Bracket missing at the Server Directive Block!");
// 			countCurlBrackets++;
// 			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
// 				error("An empty Server Block!");
// 			break;
// 		case(KEEPALIVE_TIMEOUT): {
// 			int seconds;
// 			std::string measure;
// 			std::istringstream iss(it->value);
// 			if (!(iss >> seconds)) {
// 				error("Invalid integer for keepalive_timeout!");
// 			} else {
// 				if (iss >> measure) {
// 					if (measure != "s") {
// 						error("Wrong measurement! Should be seconds(s)(keepalive timeout)");
// 					}
// 				} else if (!iss.eof()) {
// 					error("Invalid format for keepalive_timeout!");
// 				}
// 			}
// 			serv.keepalive_timeout = seconds;
// 			++it;
// 			if (it->type != SEMICOLON)
// 				error("Keepalive timeout is missing semi-colon!");
// 			break;
// 		} case(SEND_TIMEOUT): {
// 			int seconds;
// 			std::string measure;
// 			std::istringstream iss(it->value);
// 			if (!(iss >> seconds)) {
// 				error("Invalid integer for Send timeout!");
// 			} else {
// 				if (iss >> measure) {
// 					if (measure != "s") {
// 						error("Wrong measurement! Should be seconds(s) (Send timeout)");
// 					}
// 				} else if (!iss.eof()) {
// 					error("Invalid format for Send timeout!");
// 				}
// 			}
// 			serv.send_timeout = seconds;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Send timeout is missing semi-colon!");
// 			break;
// 		} case(LISTEN): {
// 			int port;
// 			std::istringstream iss(it->value);
// 			if (!(iss >> port)) {
// 				error("Invalid integer for listen!");
// 			} else if (!iss.eof()) {
// 				error("Invalid format for listen!");
// 			}
// 			serv.listen = port;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Listen dir is missing semicolon!");
// 			break;
// 		} case(SERVER_NAME):
// 			serv.server_name = it->value;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Servername dir is missing semi-colon");
// 			break;
// 		case(ROOT):
// 			serv.root = it->value;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Root dir is missing semi-colon!");
// 			break;
// 		case(AUTOINDEX):
// 			serv.autoindex = it->value;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Autoindex dir is missing semi colon!");
// 			break;
// 		case(INDEX):
// 			serv.index = it->value;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Index dir is missing semi colon!");
// 			break;
// 		case(DIR_LISTING):
// 			serv.directory_listing = it->value;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Dir Listing is missing semi colon!");
// 			break;
// 		case(CLIENT_BODY_SIZE): {
// 			long long size;
// 			std::istringstream iss(it->value);
// 			if (!(iss >> size)) {
// 				error("Invalid Num for size!");
// 			} else if (!iss.eof()) {
// 				error("Invalid format for size!");
// 			}
// 			serv.client_body_size = size;
// 			it++;
// 			if (it->type != SEMICOLON)
// 				error("Client Body Size is missing semi colon!");
// 			break;
// 		} case(LOCATION):
// 			//root and know how to receive multiple locations in case
// 			location.path = it->key;
// 			it++;
// 			if (it->type != OPEN_CURLY_BRACKET)
// 				error("Open Curly Bracket missing at the Location Block!");
// 			countCurlBrackets++;
// 			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
// 				error("An empty Location Block!");
// 			while (it->type != CLOSED_CURLY_BRACKET) {
// 				it++;
// 				if (it->type == METHODS) {
// 					std::istringstream iss(it->key);
// 					std::string	methos;
// 					while (iss >> methos) {
// 						location.methods.push_back(methos);
// 					}
// 					it++;
// 					if (it->type != SEMICOLON)
// 						error("Method dir is missing semi colon!");
// 				}
// 				if (it->type == REDIRECT) {
// 					location.redirect = it->value;
// 					it++;
// 					if (it->type != SEMICOLON)
// 						error("Method dir is missing semi colon!");
// 				} 
// 				if (it->type == ROOT) {
// 					location.root = it->value;
// 					it++;
// 					if (it->type != SEMICOLON)
// 						error("Location root is missing semi colon!");
// 				}
// 			}
// 			if (it->type != CLOSED_CURLY_BRACKET)
// 				error("Closed Curly Bracket missing at the Location Block!");
// 			countCurlBrackets--;
// 			if (countCurlBrackets == 1)
// 				serv.location.push_back(location);
// 			break ;
// 		case (CLOSED_CURLY_BRACKET):
// 			countCurlBrackets--;
// 			if (countCurlBrackets == 1)
// 				servers.push_back(serv);
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 	if (countCurlBrackets != 0)
// 		error("Curr brackets error");
// 	return (0);
// }

