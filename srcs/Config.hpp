/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:00:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/28 12:03:49 by otuyishi         ###   ########.fr       */
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
# include<cstdlib>
# include<exception>

# define red "\033[22;31m";
# define yellow "\033[01;33m";
# define green "\033[22;32m";

enum token {
	HTTP,
	SERVERBLOCK,
	KEEPALIVE_TIMEOUT, // default
	SEND_TIMEOUT, // default
	LISTEN,
	SERVER_NAME,
	ROOT,
	AUTOINDEX,
	INDEX, // default
	DIR_LISTING,
	CLIENT_BODY_SIZE, // default
	LOCATION,
	METHODS,
	REDIRECT,
	OPEN_CURLY_BRACKET,
	CLOSED_CURLY_BRACKET,
	SEMICOLON,
};

struct lexer_node {
  token type;
  std::string key;
  std::string value;
};

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
		
		std::vector<lexer_node> lexer;
		
		struct Location {
			std::vector <std::string>		methods;
			std::string						redirect;
			std::string						root;
			std::string						path;
		};

		struct	ServerConfig {
			int								keepalive_timeout;
			int								send_timeout;
			int								listen;
			std::string						server_name;
			std::string						root;
			std::string						autoindex;
			std::string						index;
			std::string						directory_listing;
			long long						client_body_size;
			std::vector<Location>			location;
		};
		ServerConfig	serv;
		Location		loc;
		void			error(const std::string& s);
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
		int			parseConfigurations(std::vector<lexer_node> lexa);
		void		set_defaults(ServerConfig &server);
		int			toInt(std::string str);
};

#endif
