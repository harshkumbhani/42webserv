/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 10:37:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/25 14:31:35 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP



#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>
#include <ostream>

#define red = "\033[22;31m";
#define yellow "\033[01;33m";
#define green "\033[22;32m";

class Config {
	private:
		//File handling var
		std::string	_ConfigFile;

		//str vars
		//Str var to store each line read from file
		std::string	_Line; //-->inside function could define this
		std::string	_Token; 

		//numeric vars
		int	_PortN;

		//boolean vars
		bool _isSSL;

		//path vars
		std::string	_RootPath;

		
	public:
		//construction n destruction
		Config();
		~Config();
		//complex data structs
		std::vector <std::string>	_IndexFiles;
		struct LocationConfig {
			std::string	path;
			std::string	dokRoot;
			std::vector <std::string>	indexFiles;
		};
		struct	ServerConfig {
			int							port;
			std::string					serverName;
			std::string					sslCert;
			std::string					sslCertKey;
			std::string					docRoot;
			std::vector <std::string>	indexFiles;
			LocationConfig	location;
		};
		std::stack <std::string> contextStack;
		std::map <std::string, ServerConfig> ServerConfigs;

		//getters
		std::string	getConfigFile(void) const;
		std::string	getLine(void) const;
		std::string	getToken(void) const;
		int			getPortN(void) const;
		bool		getIsSSL(void) const;
		std::string	getRootPath(void) const;

		//setters
		void		setConfigFile(std::string configFile);
		void		setLine(std::string setLine);
		void		setToken(std::string setToken);
		void		setPortN(int portN);
		void		setIsSSL(bool isSSL);
		void		setRootPath(std::string rootPath);

		//functions
		int			readConfigurations(std::string file);
};

#endif
