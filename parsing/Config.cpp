/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/25 15:54:28 by otuyishi         ###   ########.fr       */
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

bool		Config::getIsSSL(void) const {
	return _isSSL;
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

void		Config::setIsSSL(bool isSSL) {
	_isSSL = isSSL;
}

void		Config::setRootPath(std::string rootPath) {
	_RootPath = rootPath;
}

int		Config::readConfigurations(std::string file) {
	std::ifstream	infile(file);
	if (!infile.is_open()) {
		std::cout << "File failed to open!" << std::endl;
		return (1);
	}
	std::string		byline;
	while (std::getline(infile, byline)) {
		// std::istringstream iss(byline);
		std::cout << byline << std::endl;
		//tokenization by HARSH
	}
	infile.close();
	return (0);
}

