/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/18 12:06:32 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::requestBlock(clientState &clientData) {
	DEBUG("\n" + clientData.readString);
	if (!clientData.flagHeaderRead) {
		std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
		if (reqMethodPos != std::string::npos) {
			std::string requestLine = clientData.readString.substr(0, reqMethodPos);
			parseRequestLine(clientData, requestLine);

			std::cout << "1111{" << clientData.readString << "}1111" << std::endl;
			std::string::size_type headerEndPos = clientData.readString.find("\r\n\r\n");
			// std::cout << "{" << clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2)) << "}" << std::endl;
			
			if (headerEndPos != std::string::npos) {
				std::cout << "2222{" << clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2)) << "}2222" << std::endl;
				std::string reqHeader = clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
				parseRequestHeader(clientData, reqHeader);
				clientData.flagHeaderRead = true;

				if (headerEndPos + 4 < clientData.readString.size()) {
					clientData.bodyString.append(clientData.readString.substr(headerEndPos + 4));
				}
			}
		}
	} else if (!clientData.flagBodyRead) {
		clientData.bodyString.append(clientData.readString);
	}
	if (clientData.contentLength > 0) {
		if (static_cast<ssize_t>(clientData.bodyString.size()) >= static_cast<ssize_t>(clientData.contentLength)) {
			clientData.flagBodyRead = true;
		}
	} else {
		clientData.flagBodyRead = true;
	}
	clientData.readString.clear();
}

void HttpRequest::parseRequestLine(clientState &clientData, std::string &line) {
	std::istringstream ss(line);
	while (!ss.eof()) {
		std::string method;
		ss >> method;
		clientData.requestLine.insert(std::make_pair("method", method));
		if (method == "GET")
			clientData.method = GET;
		if (method == "POST")
			clientData.method = POST;
		if (method == "DELETE")
			clientData.method = DELETE;
		std::string url;
		ss >> url;
		clientData.requestLine.insert(std::make_pair("url", url));
		std::string httpversion;
		ss >> httpversion;
		clientData.requestLine.insert(std::make_pair("httpversion", httpversion));
	}
}

void HttpRequest::parseRequestHeader(clientState &clientData, std::string &reqheader) {
	std::istringstream ss(reqheader);
	std::string line;
	if (clientData.flagPartiallyRead == false) {
		while (std::getline(ss, line)) {
			if (!line.empty() && *line.rbegin() == '\r')
				line.erase(line.length() - 1);
			std::size_t pos = line.find(':');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				std::string::iterator it = value.begin();
				while (it != value.end() && std::isspace(*it))
					++it;
				value.erase(value.begin(), it);
				clientData.contentLength = 0;
				if (key == "Content-Length")
					clientData.contentLength = static_cast<ssize_t>(std::atol(value.c_str()));
				clientData.header.insert(std::make_pair(key, value));
			}
		}
		if (clientData.header["Connection"] == "keep-alive")
			clientData.isKeepAlive = true;
	}
}
