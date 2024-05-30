/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:00:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/30 17:25:06 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include<stack>
# include<cstring>
# include<ostream>

#include "Lexer.hpp"

struct Location {
	std::vector <std::string>		methods;
	std::string						redirect;
	std::string						root;
	std::string						index;
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
		
		//General functions
		int			parseConfigurations(std::vector<lexer_node> &lexa);

		//-->Server features
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
		void	finaliseServer(ServerConfig &server);

		//-->Location features
		void 	parseLocationBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets, ServerConfig &server);
		void 	parseMethods(std::vector<lexer_node>::iterator &it, Location &loc);
		void 	parseRedirect(std::vector<lexer_node>::iterator &it, Location &loc);
		void	parseLocationRoot(std::vector<lexer_node>::iterator &it, Location &loc);
		void	finaliseLocation(Location &loc, ServerConfig &server);
		void	parseLocationIndex(std::vector<lexer_node>::iterator &it, Location &loc);
};

std::ostream &operator<<(std::ostream &output, const Config &parser);
std::ostream &operator<<(std::ostream &output, const Location &location);
