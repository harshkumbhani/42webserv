/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelhaj- <mbelhaj-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:38:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/28 17:48:03 by mbelhaj-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(std::vector<lexer_node> lexer) : lexer(lexer) {
  parseConfigurations(this->lexer);
  parseMimeTypes(mimeTypeFilePath);
}

Parser::~Parser() {}

std::vector<ServerParser> Parser::getParser() const { return this->servers; }

//-->Server features
void Parser::parseKeepaliveTimeout(std::vector<lexer_node>::iterator &it,
                                   ServerParser &server) {
  int seconds;
  std::string measure;
  std::istringstream iss(it->value);
  if (!(iss >> seconds)) {
    throw std::runtime_error("Invalid integer for keepalive_timeout!");
  } else {
    if (iss >> measure) {
      if (measure != "s") {
        throw std::runtime_error(
            "Wrong measurement! Should be seconds(s)(keepalive timeout)");
      }
    } else if (!iss.eof()) {
      throw std::runtime_error("Invalid format for keepalive_timeout!");
    }
  }
  server.keepalive_timeout = seconds;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Keepalive timeout is missing a semi-colon!");
}

void Parser::parseSendTimeout(std::vector<lexer_node>::iterator &it,
                              ServerParser &server) {
  int seconds;
  std::string measure;
  std::istringstream iss(it->value);
  if (!(iss >> seconds)) {
    throw std::runtime_error("Invalid integer for Send timeout!");
  } else {
    if (iss >> measure) {
      if (measure != "s") {
        throw std::runtime_error(
            "Wrong measurement! Should be seconds(s) (Send timeout)");
      }
    } else if (!iss.eof()) {
      throw std::runtime_error("Invalid format for Send timeout!");
    }
  }
  server.send_timeout = seconds;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Send timeout is missing semi-colon!");
}

void Parser::parseListen(std::vector<lexer_node>::iterator &it,
                         ServerParser &server) {
  int port;
  std::istringstream iss(it->value);
  if (!(iss >> port)) {
    throw std::runtime_error("Invalid integer for listen!");
  } else if (!iss.eof()) {
    throw std::runtime_error("Invalid format for listen!");
  }
  server.listen = port;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Listen dir is missing semicolon!");
}

void Parser::parseServerName(std::vector<lexer_node>::iterator &it,
                             ServerParser &server) {
  server.server_name = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Servername dir is missing semi-colon");
}

void Parser::parseRoot(std::vector<lexer_node>::iterator &it,
                       ServerParser &server) {
  server.root = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Root dir is missing semi-colon!");
}

void Parser::parseAutoindex(std::vector<lexer_node>::iterator &it,
                            ServerParser &server) {
  server.autoindex = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Autoindex dir is missing semi colon!");
}

void Parser::parseIndex(std::vector<lexer_node>::iterator &it,
                        ServerParser &server) {
  server.index = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Index dir is missing semi colon!");
}

void Parser::parseDirListing(std::vector<lexer_node>::iterator &it,
                             ServerParser &server) {
  server.directory_listing = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Dir Listing is missing semi colon!");
}

void Parser::parseClientBodySize(std::vector<lexer_node>::iterator &it,
                                 ServerParser &server) {
  size_t size;
  std::istringstream iss(it->value);
  if (!(iss >> size)) {
    throw std::runtime_error("Invalid Num for size!");
  } else if (!iss.eof()) {
    throw std::runtime_error("Invalid format for size!");
  }
  server.client_body_size = size;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Client Body Size is missing semi colon!");
}

//-->Location features
void Parser::parseMethods(std::vector<lexer_node>::iterator &it,
                          Location &loc) {
  std::istringstream iss(it->value);
  std::string methos;
  while (iss >> methos) {
    loc.methods.push_back(methos);
  }

  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Method dir is missing semi colon!");
}

void Parser::parseRedirect(std::vector<lexer_node>::iterator &it,
                           Location &loc) {
  loc.redirect = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Method dir is missing semi colon!");
}

void Parser::parseLocationRoot(std::vector<lexer_node>::iterator &it,
                               Location &loc) {
  loc.root = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Location root is missing semi colon!");
}

void Parser::parseLocationIndex(std::vector<lexer_node>::iterator &it,
                                Location &loc) {
  loc.index = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != SEMICOLON)
    throw std::runtime_error("Index dir is missing semi colon!");
}

void Parser::finaliseLocation(Location &loc, ServerParser &server) {
  if (loc.root == "")
    loc.root = server.root;
  if (loc.index == "")
    loc.index = server.index;
}

void Parser::parseLocationBlock(std::vector<lexer_node>::iterator &it,
                                int &countCurlBrackets, ServerParser &server) {
  Location loc = (struct Location){};
  if (it->value == "")
    throw std::runtime_error("Missing Location Path");
  loc.path = it->value;
  if ((it + 1) != lexer.end() && (it + 1)->type != OPEN_CURLY_BRACKET)
    throw std::runtime_error(
        "Open Curly Bracket missing at the Location Block!");
  it++;
  if ((it + 2) != lexer.end() && (it + 2)->type == CLOSED_CURLY_BRACKET)
    throw std::runtime_error("An empty Location Block!");
  while (it != lexer.end()) {
    switch (it->type) {
    case (METHODS):
      parseMethods(it, loc);
      break;
    case (REDIRECT):
      parseRedirect(it, loc);
      break;
    case (ROOT):
      parseLocationRoot(it, loc);
      break;
    case (INDEX):
      parseLocationIndex(it, loc);
      break;
    case (SEMICOLON):
      break;
    case (OPEN_CURLY_BRACKET):
      countCurlBrackets++;
      break;
    case (CLOSED_CURLY_BRACKET):
      countCurlBrackets--;
      break;
    default:
      break;
    }
    if (countCurlBrackets == 2)
      break;
    ++it;
  }
  server.location.push_back(loc);
}
