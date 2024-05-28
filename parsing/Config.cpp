/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/27 19:40:32 by otuyishi         ###   ########.fr       */
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

// int		Config::readConfigurations(std::string file) {
// 	std::ifstream	infile(file);
// 	if (!infile.is_open()) {
// 		std::cout << "File failed to open!" << std::endl;
// 		return (1);
// 	}
// 	std::string		byline;
// 	while (std::getline(infile, byline)) {
// 		// std::istringstream iss(byline);
// 		std::cout << byline << std::endl;
// 		//tokenization by HARSH
// 	}
// 	infile.close();
// 	return (0);
// }

inline void Config::error(const std::string& str){
    throw std::runtime_error(str);
}

int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
	int countCurlBrackets = 0;
	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
		Location	location;
		switch (it->type)
		{
		case(SERVER):
			it++;
			if (it->type != OPEN_CURLY_BRACKET)
				error("Open Curly Bracket missing at the Server Directive Block!");
			countCurlBrackets++;
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
				error("Open Curly Bracket missing at the Location Directive!");
			countCurlBrackets++;
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
			} else if (it->type == REDIRECT) {
				loc.redirect = it->value;
				it++;
				if (it->type != SEMICOLON)
					error("Method dir is missing semi colon!");
			} else
				error("Empty Location Block!");
			it++;
			if (it->type != CLOSED_CURLY_BRACKET)
				error("Closed Curly Bracket missing at the Location Directive Block!");
			countCurlBrackets--;
			break ;
		case (CLOSED_CURLY_BRACKET):
			countCurlBrackets--;
			break;
		default:
			break;
		}
	}
	if (countCurlBrackets != 0)
		error("Curr brackets error");
	return (0);
}


//=======================
// location.path = it->key;
// it++;
// if (it->type == OPEN_CURLY_BRACKET) {
// 	countCurlBracket++;
// 	if (it->type == SEMICOLON)
// 		it++;
// 	if (it->type == CLOSED_CURLY_BRACKET)
// 		it++; countCurlBracket--;
// 	switch (it->type) {
// 	case (METHODS):
// 		location.methods.push_back(it->value);
// 		break;
// 	case (REDIRECT):
// 		location.redirect = it->value;
// 		break;
// 	default:
// 		break;
// 	}
// }
