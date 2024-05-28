/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/28 13:03:33 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() {
	serv.keepalive_timeout = 0;
}

Config::~Config() {}

//getters
std::string	Config::getConfigFile(void) const {
	return _ConfigFile;
}

std::string	Config::getLine(void) const {
	return _Line;
}

std::string	Config::getToken(void) const {
	return _Token;
}

int			Config::getPortN(void) const {
	return _PortN;
}

std::string	Config::getRootPath(void) const {
	return _RootPath;
}


//setters
void		Config::setConfigFile(std::string configFile) {
	_ConfigFile = configFile;
}

void		Config::setLine(std::string line) {
	_Line = line;
}

void		Config::setToken(std::string token) {
	_Token = token;
}

void		Config::setPortN(int portN) {
	_PortN = portN;
}

void		Config::setRootPath(std::string rootPath) {
	_RootPath = rootPath;
}

inline void Config::error(const std::string& str){
    throw std::runtime_error(str);
}

int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
	int countCurlBrackets = 0;
	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
		Location	location;
		switch (it->type)
		{
		case(HTTP):
			it++;
			if (it->type != OPEN_CURLY_BRACKET)
				error("Open Curly Bracket missing at the HTTP Directive Block!");
			countCurlBrackets++;
			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
				error("An empty Http Block!");
			break;
		case(SERVERBLOCK):
			it++;
			if (it->type != OPEN_CURLY_BRACKET)
				error("Open Curly Bracket missing at the Server Directive Block!");
			countCurlBrackets++;
			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
				error("An empty Server Block!");
			break;
		case(KEEPALIVE_TIMEOUT): {
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
			serv.keepalive_timeout = seconds;
			++it;
			if (it->type != SEMICOLON)
				error("Keepalive timeout is missing semi-colon!");
			break;
		} case(SEND_TIMEOUT): {
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
			serv.send_timeout = seconds;
			it++;
			if (it->type != SEMICOLON)
				error("Send timeout is missing semi-colon!");
			break;
		} case(LISTEN): {
			int port;
			std::istringstream iss(it->value);
			if (!(iss >> port)) {
				error("Invalid integer for listen!");
			} else if (!iss.eof()) {
				error("Invalid format for listen!");
			}
			serv.listen = port;
			it++;
			if (it->type != SEMICOLON)
				error("Listen dir is missing semicolon!");
			break;
		} case(SERVER_NAME):
			serv.server_name = it->value;
			it++;
			if (it->type != SEMICOLON)
				error("Servername dir is missing semi-colon");
			break;
		case(ROOT):
			serv.root = it->value;
			it++;
			if (it->type != SEMICOLON)
				error("Root dir is missing semi-colon!");
			break;
		case(AUTOINDEX):
			serv.autoindex = it->value;
			it++;
			if (it->type != SEMICOLON)
				error("Autoindex dir is missing semi colon!");
			break;
		case(INDEX):
			serv.index = it->value;
			it++;
			if (it->type != SEMICOLON)
				error("Index dir is missing semi colon!");
			break;
		case(DIR_LISTING):
			serv.directory_listing = it->value;
			it++;
			if (it->type != SEMICOLON)
				error("Dir Listing is missing semi colon!");
			break;
		case(CLIENT_BODY_SIZE): {
			long long size;
			std::istringstream iss(it->value);
			if (!(iss >> size)) {
				error("Invalid Num for size!");
			} else if (!iss.eof()) {
				error("Invalid format for size!");
			}
			serv.client_body_size = size;
			it++;
			if (it->type != SEMICOLON)
				error("Client Body Size is missing semi colon!");
			break;
		} case(LOCATION):
			//root and know how to receive multiple locations in case
			location.path = it->key;
			it++;
			if (it->type != OPEN_CURLY_BRACKET)
				error("Open Curly Bracket missing at the Location Block!");
			countCurlBrackets++;
			if (it->type == OPEN_CURLY_BRACKET && (it + 1)->type == CLOSED_CURLY_BRACKET)
				error("An empty Location Block!");
			while (it->type != CLOSED_CURLY_BRACKET) {
				it++;
				if (it->type == METHODS) {
					std::istringstream iss(it->key);
					std::string	methos;
					while (iss >> methos) {
						loc.methods.push_back(methos);
					}
					it++;
					if (it->type != SEMICOLON)
						error("Method dir is missing semi colon!");
				}
				if (it->type == REDIRECT) {
					loc.redirect = it->value;
					it++;
					if (it->type != SEMICOLON)
						error("Method dir is missing semi colon!");
				} 
				if (it->type == ROOT) {
					loc.root = it->value;
					it++;
					if (it->type != SEMICOLON)
						error("Location root is missing semi colon!");
				}
			}
			if (it->type != CLOSED_CURLY_BRACKET)
				error("Closed Curly Bracket missing at the Location Block!");
			countCurlBrackets--;
			if (countCurlBrackets == 1)
				serv.location.push_back(loc);
			break ;
		case (CLOSED_CURLY_BRACKET):
			countCurlBrackets--;
			if (countCurlBrackets == 1)
				servers.push_back(serv);
			break;
		default:
			break;
		}
	}
	if (countCurlBrackets != 0)
		error("Curr brackets error");
	return (0);
}
