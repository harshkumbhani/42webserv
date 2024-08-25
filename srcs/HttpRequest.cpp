/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/21 13:04:19 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::requestBlock(clientState &clientData) {
	if (clientData.flagHeaderRead == false) {
		std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
		if (reqMethodPos != std::string::npos) {
			std::string requestLine = clientData.readString.substr(0, reqMethodPos);

			parseRequestLine(clientData, requestLine);
			std::string::size_type headerEndPos = clientData.readString.find("\r\n\r\n");

			if (headerEndPos != std::string::npos && headerEndPos > reqMethodPos + 2) {
				std::string reqHeader = clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
				parseRequestHeader(clientData, reqHeader);
				clientData.flagHeaderRead = true;
				clientData.bodyString.append(clientData.readString.substr(headerEndPos + 4));
				clientData.readString.clear();
			}
		}
	} else if (!clientData.flagBodyRead) {
		clientData.bodyString.append(clientData.readString);
	}

	std::map<std::string, std::string>::iterator contentLengthIt = clientData.header.find("Content-Length");
	if (contentLengthIt != clientData.header.end()) {
		clientData.contentLength = static_cast<ssize_t>(std::atol(contentLengthIt->second.c_str()));
		ServerParser	parserInstance;
		clientData.flagFileSizeTooBig = clientData.contentLength > static_cast<ssize_t>(parserInstance.client_body_size);
		if (static_cast<ssize_t>(clientData.bodyString.size()) == static_cast<ssize_t>(clientData.contentLength))
			clientData.flagBodyRead = true;
	}
	clientData.readString.clear();
}

void HttpRequest::parseRequestLine(clientState &clientData, std::string &line) {
	std::istringstream ss(line);
	std::string method, url, httpversion;

	ss >> method;
	ss >> url;
	ss >> httpversion;

	if (method == "POST")
		clientData.method = POST;
	else if (method == "GET")
		clientData.method = GET;
	else if (method == "DELETE")
		clientData.method = DELETE;
	else
		clientData.method = DEFAULT;

	clientData.requestLine.push_back(method);
	clientData.requestLine.push_back(url);
	clientData.requestLine.push_back(httpversion);
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
		if (clientData.header.find("Connection") != clientData.header.end() && clientData.header["Connection"] == "keep-alive") {
			clientData.isKeepAlive = true;
		}
	}
}
