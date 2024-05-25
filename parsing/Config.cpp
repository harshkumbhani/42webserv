/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/25 18:54:16 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() {}

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

void	Config::set_defaults(ServerConfig &server) {
	//defaults
}

int		Config::parseConfigurations(std::vector<struct x> token) {
	
	for (std::vector<struct x>::iterator it = token.begin(); it != token.end(); ++it) {
		ServerConfig server;
		set_defaults(server);
		switch (x.type)
		{
		
		case(keepalive_timeout):
			server.keepalive_timeout = x.key;
			break;
		default:
			break;
		}
		if (x.type == keepalive_timeout)
	}
}
