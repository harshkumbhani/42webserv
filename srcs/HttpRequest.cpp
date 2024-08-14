/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkumbhan <hkumbhan@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/14 09:56:55 by hkumbhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "HttpRequest.hpp"

//HttpRequest::HttpRequest() {}

//HttpRequest::~HttpRequest() {}

//// Request Line
//// Header
//// Body-->optional
//void HttpRequest::requestBlock(clientState &clientData) {
//  HttpRequest httpReq;

//  if (clientData.flagHeaderRead == false) {
//    std::string::size_type headerEndPos =
//        clientData.readString.find("\r\n\r\n");
//    std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
//    std::string requestLine = clientData.readString.substr(0, reqMethodPos);
//    httpReq.parseRequestLine(clientData, requestLine);

//    std::string reqHeader = clientData.readString.substr(
//        reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
//    httpReq.parseRequestHeader(clientData, reqHeader);
//    clientData.readString = clientData.readString.substr(headerEndPos + 4);
//    INFO("Request Parsing successfull");
//  }
//  if (clientData.method == POST)
//    httpReq.parseRequestBody(clientData, clientData.readString);
//}

//// POST /submit-form HTTP/1.1\r\n
//// METHOD SP URI SP HTTP-VERSION CRLF
//void HttpRequest::parseRequestLine(clientState &clientData, std::string &line) {
//  std::stringstream ss(line);
//  while (!ss.eof()) {
//    std::string method;
//    ss >> method;
//    clientData.requestLine.insert(std::make_pair("method", method));

//    if (method == "POST")
//      clientData.method = POST;

//    std::string url;
//    ss >> url;
//    std::cout << "URL: " << url << std::endl;
//    clientData.requestLine.insert(std::make_pair("url", url));

//    std::string httpversion;
//    ss >> httpversion;
//    clientData.requestLine.insert(std::make_pair("httpversion", httpversion));
//  }
//  if (clientData.requestLine["url"] == "/assets/bg4.mp4")
//    std::cout << clientData.readString << std::endl;
//}

//// Host: www.example.com\r\n
//// Content-Type: application/x-www-form-urlencoded\r\n
//// Content-Length: 27\r\n
//// \r\n
//void HttpRequest::parseRequestHeader(clientState &clientData,
//                                     std::string &reqheader) {
//  std::stringstream ss(reqheader);
//  std::string line;
//  while (std::getline(ss, line)) {
//    if (!line.empty() && *line.rbegin() == '\r')
//      line.erase(line.length() - 1);
//    std::size_t pos = line.find(':');
//    if (pos != std::string::npos) {
//      std::string key = line.substr(0, pos);
//      std::string value = line.substr(pos + 1);
//      std::string::iterator it = value.begin();
//      while (it != value.end() && std::isspace(*it))
//        ++it;
//      value.erase(value.begin(), it);
//      clientData.header.insert(std::make_pair(key, value));
//    }
//  }
//  clientData.flagHeaderRead = true;
//}

//// bodylen=23
//// key1=value1&key2=value2
//void HttpRequest::parseRequestBody(clientState &clientData,
//                                   std::string &request) {
//  std::map<std::string, std::string>::const_iterator it =
//      clientData.header.find("Content-Length");
//  if (it != header.end())
//    clientData.contentLength = atoi(it->second.c_str());
//  else
//    clientData.contentLength = 0;
//  if (clientData.contentLength > 0) {
//    if (request.size() > 0)
//      clientData.bodyString += request;
//    std::string::size_type pos = request.find("\r\n\r\n");
//    if (pos != std::string::npos)
//      clientData.flagBodyRead = true;
//  }
//}

//// My Notes
//// Requests
////-->GET<--
//// purpose: retrieves data, parameters in URL, no body
//// mandatory method('GET'),
////  path('/search?q=example'),
////  http version('HTTP/1.1'),
////  end of headers('\r\n\r\n')
//// optional headers(host, user_agent,accept)

////-->POST<--
//// purpose: sends data to server, parameters in body, creates/updates resources
//// mandatory method('POST'),
//// path('/submit-form'),1
//// http version('HTTP/1.1'),
//// headers(host, content-length, user_agent, content-type),
//// end of headers('\r\n\r\n')
//// body('name=John+Doe&age=30&city=NY')

////-->DELETE<--
//// requests resource deletion, no body
//// mandatory method('DELETE'),
//// path('/delete-profile/123'),
//// http version('HTTP/1.1'),
//// end of headers('\r\n\r\n')
//// optional headers(host, user_agent,accept)

//// POST /submit-form HTTP/1.1\r\n
//// Host: www.example.com\r\n
//// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36
//// (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\n Accept:
//// text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
//// Accept-Language: en-US,en;q=0.5\r\n
//// Accept-Encoding: gzip, deflate, br\r\n
//// Content-Type: application/x-www-form-urlencoded\r\n
//// Content-Length: 27\r\n
//// Connection: keep-alive\r\n
//// Upgrade-Insecure-Requests: 1\r\n
//// \r\n
//// key1=value1&key2=value2

//// parse end of buffer

//// requestLine
////-->vector["POST", "/submit-form", "HTTP/1.1"]======>till '''\r\n'''

//// Header
//// map{
////		User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)
//// AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\n
////  		Accept:
////  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
////  		Accept-Language: en-US,en;q=0.5\r\n
////  		Accept-Encoding: gzip, deflate, br\r\n
////  		Content-Type: application/x-www-form-urlencoded\r\n
////  		Content-Length: 27\r\n
////  		Connection: keep-alive\r\n
////  		Upgrade-Insecure-Requests: 1\r\n
////  }-->fromatted as Key:Value, key up to ':' and value up to '\r\n'
////=======> header ends with '\r\n\r\n'

//// Body
//// string


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
	DEBUG("\n==============================================\n" + clientData.bodyString + "\n==============================================\n");
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
		if (clientData.header["Connection"] == "keep-alive")
			clientData.isKeepAlive = true;
	}
}
