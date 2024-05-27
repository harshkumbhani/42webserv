/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/27 10:41:21 by otuyishi         ###   ########.fr       */
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

int	Config::toInt(std::string str) {
	std::istringstream iss(str);
	int num;
	iss >> num;
	if (iss.fail()) {
		std::cerr << "Num Conversion failed for: " << str << std::endl;
		return 0;
	}
	return num;
}

inline void Config::error(const std::string& s){
    throw runtime_error(s);
}

int		Config::parseConfigurations(std::vector<lexer_node> lexa) {
	for (std::vector<lexer_node>::iterator it = lexa.begin(); it != lexa.end(); ++it) {
		Location	location;
		// set_defaults(server);
		int countCurlBrackets = 0;
		switch (it->type)
		{
		case(SERVER):
			it++;
			if (it->type != OPEN_CURLY_BRACKET) {
				std::cout << "Missing Open Curly Bracket at the Server Directive!" << std::endl;
				return (1);
			}
			countCurlBrackets++;
			break;
		case(KEEPALIVE_TIMEOUT):
			it++;
			if (it->type != SEMICOLON) {
				std::cout << "Missing Semi-Colon at Keepalive timeout!" << std::endl;
				return (1);
			}
			serv.keepalive_timeout = toInt(it->value);
			break;
		case(SEND_TIMEOUT):
			serv.send_timeout = toInt(it->value);
			break;
		case(LISTEN):
			serv.listen = toInt(it->value);
			break;
		case(SERVER_NAME):
			serv.server_name = it->value;
			break;
		case(ROOT):
			serv.root = it->value;
			break;
		case(AUTOINDEX):
			serv.autoindex = it->value;
			break;
		case(INDEX):
			serv.index = it->value;
			break;
		case(DIR_LISTING):
			serv.directory_listing = it->value;
			break;
		case(CLIENT_BODY_SIZE):
			serv.client_body_size = toInt(it->value); //gonna have prob here, converting to int not enough for long long var
			break;
		case(LOCATION):
			location.path = it->key;
			break ;
		case(OPEN_CURLY_BRACKET):
			break;
		case (METHODS):
			std::istringstream iss(it->key);
			std::string	methos;
			while (iss >> methos)
				loc.methods.push_back(methos);
			break;
		case (REDIRECT):
			loc.redirect = it->value;
			break;
		case(CLOSED_CURLY_BRACKET):
			break;
		case(SEMICOLON):
			break;
		default:
			break;
		}
	}
	if (countCurlBracket != 0) {
		std::cout << "Curr brackets error" << std::endl;
		return (1);
	}
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