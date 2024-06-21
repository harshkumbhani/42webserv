/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:08:24 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/21 12:27:22 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

std::string	HttpResponse::respond_Get(HttpRequest const &req) {
	std::map<std::string, std::string>::const_iterator	url = req.ReqLine.find("url");
	if (url != req.ReqLine.end()) {
		std::string	route = "./www" + url->second;
		std::ifstream route_file(route.c_str());
		if (route_file.fail() == true) {
			_StatusLine = req.ReqLine.find("httpversion")->second + " " + "404" + " Not Found\r\n";
			_Header = "Content-Type: text/html\r\n";
			std::string	pageNotFound = "./www/text.html";
			std::ifstream page(pageNotFound.c_str());
			if (!page.is_open()) {
				page.close();
				throw std::runtime_error("ErrorPage is Missing");
			}
			std::stringstream buffer;
			buffer << page.rdbuf();
			std::string pageContent = buffer.str();
			page.close();
			
			std::stringstream ss;
			ss << pageContent.size();
			std::string	pageSize;
			ss >> pageSize;

			_Header = _Header + "Content-Length: " + pageSize + "\r\nConnection: close\r\n";
			_Body = pageContent;
		} else {
			std::string	buffer;
			buffer.assign(std::istreambuf_iterator<char>(route_file), std::istreambuf_iterator<char>());

			_StatusLine = req.ReqLine.find("httpversion")->second + " " + "200" + " OK\r\n";
			_Header = "Content-Type: " + url->second + "\r\n";

			std::stringstream ss;
			ss << buffer.size();
			std::string	fileSize;
			ss >> fileSize;

			_Header = _Header + "Content-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
			_Body = buffer;
			route_file.close();
		}
		time_t now = time(0);
		char	buf[100];
		strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
		_Header = _Header + "Date: " + std::string(buf) + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n\r\n";
	}
	_Response = _StatusLine + _Header + _Body;
	return (_Response);
}

void	HttpResponse::response_Post(HttpRequest const &req) {
	(void)req;
}

void	HttpResponse::respond(HttpRequest const &req) {
	std::map<std::string, std::string>::const_iterator	metho = req.ReqLine.find("method");
	if (metho != req.ReqLine.end() && metho->second == "GET") {
		respond_Get(req);
	} else if (metho->second == "POST") {
		response_Post(req);
	} else if (metho->second == "DELETE") {
		;//response_Delete(req);
	}
}
