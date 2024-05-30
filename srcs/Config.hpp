/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:00:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/30 11:57:55 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include<stack>
# include<cstring>
# include<ostream>

#include "Lexer.hpp"

# define red "\033[22;31m";
# define yellow "\033[01;33m";
# define green "\033[22;32m";

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
	size_t							client_body_size;
	std::vector<Location>			location;
};

class Config {
	private:
		std::vector<lexer_node> lexer;
		std::vector<ServerConfig>	servers;
	
	public:
		//construction n destruction
		Config(std::vector<lexer_node> lexer);
		~Config();

		// getter
		std::vector<ServerConfig> getParser() const;
		
		//functions
		int			parseConfigurations(std::vector<lexer_node> lexa);
		void		error(const std::string& s);

		//void parseLocationRoot(std::vector<lexer_node>::iterator &it);
		void 	parseHttpBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets);
		void 	parseServerBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets);
		void 	parseKeepaliveTimeout(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseSendTimeout(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseListen(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseServerName(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseRoot(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseAutoindex(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseIndex(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseDirListing(std::vector<lexer_node>::iterator &it, ServerConfig &server);
		void 	parseClientBodySize(std::vector<lexer_node>::iterator &it, ServerConfig &server);

		void 	parseLocationBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets, ServerConfig &server);
		void 	parseMethods(std::vector<lexer_node>::iterator &it, Location &loc);
		void 	parseRedirect(std::vector<lexer_node>::iterator &it, Location &loc);
		void	parseLocationRoot(std::vector<lexer_node>::iterator &it, Location &loc);

		// void	openCurlyBracket(int &countCurlBrackets);
		// void	closedCurlyBracket(int &countCurlBrackets);
		// void	semiColon(std::vector<lexer_node>::iterator it);
};

std::ostream &operator<<(std::ostream &output, const Config &parser);
std::ostream &operator<<(std::ostream &output, const Location &location);
