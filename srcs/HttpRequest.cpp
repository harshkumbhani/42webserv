/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harsh <harsh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/16 11:48:45 by harsh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::requestBlock(clientState &clientData) {
	// DEBUG("111111111111111111111111\n");
	if (!clientData.flagHeaderRead) {
		std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
		if (reqMethodPos != std::string::npos) {
			std::string requestLine = clientData.readString.substr(0, reqMethodPos);
			// DEBUG(requestLine);
			parseRequestLine(clientData, requestLine);
			DEBUG("\n==============================================\n" + requestLine + "\n==============================================\n");
			
			// DEBUG("222222222222222222222222\n");
			std::string::size_type headerEndPos = clientData.readString.find("\r\n\r\n");
			if (headerEndPos != std::string::npos && headerEndPos > reqMethodPos + 2) {
				std::string reqHeader = clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
				parseRequestHeader(clientData, reqHeader);
				DEBUG("\n==============================================\n" + reqHeader + "\n==============================================\n");
				clientData.flagHeaderRead = true;

				clientData.bodyString.append(clientData.readString.substr(headerEndPos + 4));
				clientData.readString.clear();
			}
		}
	} else {
		clientData.bodyString.append(clientData.readString);
	}
	std::map<std::string, std::string>::iterator contentLengthIt = clientData.header.find("Content-Length");
	if (contentLengthIt != clientData.header.end()) {
		clientData.contentLength = static_cast<ssize_t>(std::atol(contentLengthIt->second.c_str()));
		DEBUG(static_cast<ssize_t>(clientData.bodyString.size()));
		DEBUG(static_cast<ssize_t>(clientData.contentLength));
		if (static_cast<ssize_t>(clientData.bodyString.size()) < static_cast<ssize_t>(clientData.contentLength))
			clientData.flagBodyRead = false;
	} else {
		clientData.flagBodyRead = true;
	}
	// DEBUG("\n==============================================\n" + clientData.bodyString + "\n==============================================\n");
	
	clientData.readString.clear();
}

// void HttpRequest::parseRequestLine(clientState &clientData, std::string &line) {
// 	std::istringstream stream(line);
// 	std::string method, url, httpVersion;

// 	if (stream >> method >> url >> httpVersion) {
// 		clientData.requestLine["method"] = method;
// 		clientData.requestLine["url"] = url;
// 		clientData.requestLine["httpversion"] = httpVersion;
// 	} else {
// 		std::cerr << "Failed to parse request line: " << line << std::endl;
// 	}
// }

void HttpRequest::parseRequestLine(clientState &clientData, std::string &line) {
	std::istringstream ss(line);
	// DEBUG("444444444444444444444444\n");
	while (!ss.eof()) {
		std::string method;
		ss >> method;
		if (method == "POST")
			clientData.method = POST;
		else if (method == "GET")
			clientData.method = GET;
		else if (method == "DELETE")
			clientData.method = DELETE;
		else if (method == "CGI")
			clientData.method = CGI;
		else
			clientData.method = DEFAULT;
		clientData.requestLine.insert(std::make_pair("method", method));
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
				clientData.header.insert(std::make_pair(key, value));
			}
		}
		if (clientData.header.find("Connection") != clientData.header.end() && clientData.header["Connection"] == "keep-alive") {
			clientData.isKeepAlive = true;
		}
	}
}

// Host: localhost:8080
