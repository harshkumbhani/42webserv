#pragma once

#include "Lexer.hpp"
#include "Utils.hpp"

const std::string mimeTypeFilePath = "./config/mime.typ";

class Parser {
	private:
		std::vector<lexer_node> lexer;
		std::vector<ServerParser> servers;

	public:
		// construction n destruction
		Parser(std::vector<lexer_node> lexer);
		~Parser();

		// getter
		std::vector<ServerParser> getParser() const;

		// General functions
		void parseConfigurations(std::vector<lexer_node> &lexa);

		//-->Server features
		void parseHttpBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets);
		void parseServerBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets);
		void parseKeepaliveTimeout(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseSendTimeout(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseListen(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseServerName(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseRoot(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseAutoindex(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseIndex(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseDirListing(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void parseClientBodySize(std::vector<lexer_node>::iterator &it, ServerParser &server);
		void finaliseServer(ServerParser &server);

		//-->Location features
		void parseLocationBlock(std::vector<lexer_node>::iterator &it, int &countCurlBrackets, ServerParser &server);
		void parseMethods(std::vector<lexer_node>::iterator &it, Location &loc);
		void parseRedirect(std::vector<lexer_node>::iterator &it, Location &loc);
		void parseLocationRoot(std::vector<lexer_node>::iterator &it, Location &loc);
		void finaliseLocation(Location &loc, ServerParser &server);
		void parseLocationIndex(std::vector<lexer_node>::iterator &it, Location &loc);
};

std::ostream &operator<<(std::ostream &output, const std::vector<ServerParser> &nodes);
std::ostream &operator<<(std::ostream &output, const Location &location);
std::ostream &operator<<(std::ostream &output, const ServerParser &nodes);
