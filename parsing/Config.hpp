/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 10:37:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/25 18:32:51 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include<iostream>
# include<map>
# include<vector>
# include<stack>
# include<fstream>
# include<string>
# include<cstring>
# include<sstream>
# include<ostream>

# define red "\033[22;31m";
# define yellow "\033[01;33m";
# define green "\033[22;32m";

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

		//path vars
		std::string	_RootPath;

		
	public:
		//construction n destruction
		Config();
		~Config();
		//complex data structs
		std::vector <std::string>	_IndexFiles;
		
		
		struct Location {
			std::vector<std::string>		methods;
			std::string						redirect;
		};

		struct	ServerConfig {
			int								keepalive_timeout;
			int								send_timeout;
			int								listen;
			std::string						server_name;
			std::string						root;
			bool							autoindex;
			std::string						index;
			bool							directory_listing;
			long long						client_body_size;
			std::vector<Location>			location;
		};

		std::vector <ServerConfig>	servers;

		std::stack <std::string> contextStack;

		std::map <std::string, ServerConfig> ServerConfigs;

		//getters
		std::string	getConfigFile(void) const;
		std::string	getLine(void) const;
		std::string	getToken(void) const;
		int			getPortN(void) const;
		std::string	getRootPath(void) const;

		//setters
		void		setConfigFile(std::string configFile);
		void		setLine(std::string setLine);
		void		setToken(std::string setToken);
		void		setPortN(int portN);
		void		setRootPath(std::string rootPath);

		//functions
		int			readConfigurations(std::string file);
		int			parseConfigurations(std::vector<struct x> token);
		void		set_defaults(ServerConfig &server);
};

#endif
