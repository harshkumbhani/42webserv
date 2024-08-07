/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkumbhan <hkumbhan@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/07 10:09:14 by hkumbhan         ###   ########.fr       */
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

		std::map<std::string, std::string> reqLine;
		std::map<std::string, std::string> header;
		std::string body;

		static void requestBlock(clientState &clientData);

		static void parseRequestLine(clientState &clientData, std::string request);
		static void parseRequestHeader(clientState &clientData, std::string &reqheader, std::string &readBody);
		//static void parseRequestBody(clientState &clientData);
		static std::string		getBoundary(clientState &clientData, std::string const &contentType);
		static	void	parseMultipartFormData(clientState &clientData, std::string &readBody);
};

#endif