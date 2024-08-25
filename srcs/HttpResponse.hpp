/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkumbhan <hkumbhan@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:09:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/25 20:04:53 by hkumbhan         ###   ########.fr       */
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

		std::string	metaData(clientState &clientData);
		std::string	webserverStamp(void);
		std::string	errorHandlingGet(int code, clientState &clientData);

		std::string statusCodes(int code);
		std::string generateErrorPage(int code, const std::string& message);
		std::string generateHtml(int code, const std::string& codeMessage);
		std::string respond(clientState &clientData);
		std::string successHandling(int statusCode, clientState &clientData, const std::string &messageBody = "");

		std::string respond_Get(clientState &clientData);
		std::string response_Post(clientState &clientData);
		std::string responseDelete(clientState &clientData);
		std::string respondRedirect(clientState &clientData);

		bool is_valid_str(const std::string &str);
		bool is_valid_char(char c);

		std::string errorHandlingPost(int statusCode, clientState &clientData);
		bool 		write_to_file(clientState &clientData, const std::string& path, const std::string& content);
		void		parse_headers(std::istringstream& contentStream, std::string& fileName, std::string& fileContent);
		std::string	findBoundary(const std::map<std::string, std::string>& headers);
		void		parseRequestBody(clientState &clientData);
		std::string	generateHttpResponse(int statusCode, const std::string& message);
};

#endif
