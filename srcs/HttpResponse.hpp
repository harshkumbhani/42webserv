/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:09:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/18 10:49:06 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "EventLogger.hpp"
#include "HttpRequest.hpp"
// #include "NewRequest.hpp"
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>
#include "Utils.hpp"

enum status { OK = 200, NOT_FOUND = 404, BAD_REQUEST = 400 };

class HttpRequest;

class HttpResponse {
	public:
		HttpResponse();
		~HttpResponse();

		std::string _StatusLine;
		std::string _Header;
		std::string _Body;
		std::string _Response;

		std::string	metaData(clientState &req);
		std::string	webserverStamp(void);
		std::string	errorHandlingGet(int code, clientState &req);

		std::string statusCode(int code);
		std::string generateHtml(int code, const std::string& codeMessage);
		std::string respond(clientState &req);
		std::string successHandling(int statusCode, clientState &req, const std::string &messageBody = "");
		// std::string handleRequestWithRange(clientState &req, const std::string &route);

		std::string respond_Get(clientState &req);
		std::string response_Post(clientState &req);
		std::string response_Delete(clientState &req);

		// supporting funcs
		bool is_valid_str(const std::string &str);
		bool is_valid_char(char c);

		//post
		std::string errorHandlingPost(int statusCode, clientState &req);
		void		write_to_file(const std::string& path, const std::string& content);
		void		parse_headers(std::istringstream& contentStream, std::string& fileName, std::string& fileContent);
		std::string	findBoundary(const std::map<std::string, std::string>& headers);
		void		parseRequestBody(clientState &clientData);
};

#endif
