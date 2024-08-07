/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/07 20:25:35 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::requestBlock(clientState &clientData) {
	std::string requestLine;
	std::string reqHeader;
	std::string readBody;
	if (clientData.flagHeaderRead == false) {
		std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
		if (reqMethodPos != std::string::npos) {
			requestLine = clientData.readString.substr(0, reqMethodPos);
			parseRequestLine(clientData, requestLine);
			
			std::string::size_type headerEndPos = clientData.readString.find("\r\n\r\n");
			if (headerEndPos != std::string::npos && headerEndPos > reqMethodPos + 2) {
				reqHeader = clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
				readBody = clientData.readString.substr(headerEndPos + 4);
				parseRequestHeader(clientData, reqHeader, readBody);
			}
		}
	}
}

void HttpRequest::parseRequestLine(clientState &clientData, std::string line) {
	std::istringstream ss(line);
	while (!ss.eof()) {
		std::string method;
		ss >> method;
		clientData.requestLine.insert(std::make_pair("method", method));
		if (method == "POST")
			clientData.method = POST;
		std::string url;
		ss >> url;
		clientData.requestLine.insert(std::make_pair("url", url));
		std::string httpversion;
		ss >> httpversion;
		clientData.requestLine.insert(std::make_pair("httpversion", httpversion));
	}
}

void HttpRequest::parseRequestHeader(clientState &clientData, std::string &reqheader, std::string &readBody) {
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
		clientData.flagHeaderRead = true;
		if (clientData.header["Connection"] == "keep-alive")
			clientData.isKeepAlive = true;
	}
	if (clientData.method == POST) {
		std::map<std::string, std::string>::iterator contentLengthIt = clientData.header.find("Content-Length");
		if (contentLengthIt != clientData.header.end())
			clientData.contentLength = static_cast<ssize_t>(std::atol(contentLengthIt->second.c_str()));
		//else {
		// 	WARNING("Content-Length header is missing");
		// 	return ;
		// }

		// std::map<std::string, std::string>::iterator contentTypeIt = clientData.header.find("Content-Type");
		// if (contentTypeIt != clientData.header.end()) {
		// 	clientData.contentType = contentTypeIt->second;
		// 	if (clientData.contentType.empty() || clientData.contentType.find("multipart/form-data") == std::string::npos) {
		// 		WARNING("Content Type is Empty or not multipart/form-data");
		// 	}
		// } else {
		// 	WARNING("Content-Type header is missing");
		// 	return;
		// }
		parseRequestBody(clientData, readBody);
	}
}

void write_to_file(const std::string& path, const std::string& content) {
	std::ofstream outFile(path.c_str(), std::ios::binary);
	if (!outFile) {
		std::cerr << "Error: Unable to open file for writing: " << path << std::endl;
		return;
	}
	outFile.write(content.c_str(), content.size());
	outFile.close();
	if (!outFile)
		std::cerr << "Error: Failed to write file: " << path << std::endl;
}

void parse_headers(std::istringstream& contentStream, std::string& fileName, std::string& fileContent) {
	std::string line;
	bool headerParsed = false;

	while (std::getline(contentStream, line)) {
		if (line == "\r" || line.empty()) {
			headerParsed = true;
			break;
		}
		if (line.find("filename=\"") != std::string::npos) {
			std::size_t startPos = line.find("filename=\"") + 10;
			std::size_t endPos = line.find("\"", startPos);
			if (endPos != std::string::npos) {
				fileName = line.substr(startPos, endPos - startPos);
			}
		}
	}
	if (!headerParsed) {
		std::cerr << "Error: Incomplete headers, waiting for more data." << std::endl;
		return;
	}
	fileContent.assign((std::istreambuf_iterator<char>(contentStream)), std::istreambuf_iterator<char>());
}

void HttpRequest::parseRequestBody(clientState &clientData, std::string &readBody) {
	clientData.bodyString.append(readBody);
	// if (clientData.contentLength > static_cast<ssize_t>(clientData.bodyString.size()))
	// 	return;
	std::string boundary = "--" + clientData.boundary;
	while (true) {
		std::size_t boundaryStart = clientData.bodyString.find(boundary);
		if (boundaryStart == std::string::npos)
			return;
		std::size_t boundaryEnd = clientData.bodyString.find("\r\n", boundaryStart);
		if (boundaryEnd == std::string::npos)
			return;
		boundaryEnd += 2;
		std::size_t nextBoundaryStart = clientData.bodyString.find(boundary, boundaryEnd);
		if (nextBoundaryStart == std::string::npos)
			return;
		std::string bodyPart = clientData.bodyString.substr(boundaryEnd, nextBoundaryStart - boundaryEnd);
		std::istringstream contentStream(bodyPart);
		std::string fileName;
		std::string fileContent;

		parse_headers(contentStream, fileName, fileContent);
		if (fileName.empty()) {
			std::cerr << "Error: No filename found in the headers." << std::endl;
			return;
		}
		std::string filePath = "./www/upload/Files/" + fileName;
		write_to_file(filePath, fileContent);
		clientData.fileName = fileName;
		clientData.bodyString.erase(0, nextBoundaryStart);
		clientData.flagBodyRead = true;
		std::cout << "File saved to: " << filePath << std::endl;
		std::cout << "File content:\n" << fileContent << std::endl;
	}
}

