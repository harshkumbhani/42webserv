/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harsh <harsh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/16 11:20:22 by harsh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#ifndef HTTPREQUEST_HPP
//#define HTTPREQUEST_HPP

//#include "Structs.hpp"
//#include "EventLogger.hpp"
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <unistd.h>

//class HttpRequest {
//public:
//  HttpRequest();
//  ~HttpRequest();

//  std::map<std::string, std::string> reqLine;
//  std::map<std::string, std::string> header;
//  std::string body;

//  static void requestBlock(clientState &clientData);

//  void parseRequestLine(clientState &clientData, std::string &request);
//  void parseRequestHeader(clientState &clientData, std::string &reqheader);
//  void parseRequestBody(clientState &clientData, std::string &request);

//private:
//};

//#endif

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "Structs.hpp"
#include "EventLogger.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		// std::map<std::string, std::string> reqLine;
		// std::map<std::string, std::string> header;
		// std::string body;

		static void	requestBlock(clientState &clientData);
		static void	parseRequestLine(clientState &clientData, std::string &request);
		static void	parseRequestHeader(clientState &clientData, std::string &reqheader);
		// static void			parseRequestBody(clientState &clientData);
		// static std::string	findBoundary(const std::map<std::string, std::string>& headers);
		// static void			parse_headers(std::istringstream& contentStream, std::string& fileName, std::string& fileContent);
		// static void			write_to_file(const std::string& path, const std::string& content);
};

#endif