/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkumbhan <hkumbhan@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/07 14:52:37 by hkumbhan         ###   ########.fr       */
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
	std::string requestLine;
	std::string reqHeader;
	std::string readBody;
	//DEBUG("\n" + clientData.readString);
	if (clientData.flagHeaderRead == false) {
		std::string::size_type reqMethodPos = clientData.readString.find("\r\n");
		
		if (reqMethodPos != std::string::npos) {
			requestLine = clientData.readString.substr(0, reqMethodPos);
			parseRequestLine(clientData, requestLine);
			
			std::string::size_type headerEndPos = clientData.readString.find("\r\n\r\n");
			if (headerEndPos != std::string::npos && headerEndPos > reqMethodPos + 2) {
				reqHeader = clientData.readString.substr(reqMethodPos + 2, headerEndPos - (reqMethodPos + 2));
				readBody = clientData.readString.substr(headerEndPos + 4);
				//DEBUG("\n1\n\n" + requestLine + "\n\n");
				//DEBUG("\n2\n\n" + reqHeader + "\n\n");
				//DEBUG("\n3\n\n" + readBody + "\n\n");
				//DEBUG("\n\n Content length of the body: " << readBody.size() << "\n\n");
				parseRequestHeader(clientData, reqHeader, readBody);
			}
		}
	}
	// DEBUG("\nREQLINE:\n\n\n" + requestLine);
	// DEBUG("\nHEADER:\n\n\n" + reqHeader);
	// DEBUG("\nBODY:\n\n\n" + readBody);
	std::cout << "-----------------End of Request-----------------------\n";
	//DEBUG("\n" + clientData.readString);
	std::cout << "------------------------------------------------------\n";
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
		// std::cout << "URL: " << url << std::endl;
		clientData.requestLine.insert(std::make_pair("url", url));
		std::string httpversion;
		ss >> httpversion;
		clientData.requestLine.insert(std::make_pair("httpversion", httpversion));
		// DEBUG(httpversion + " \nHALOHALOGHALO");
	}
	// if (clientData.requestLine["url"] == "/assets/bg4.mp4")
	// 	std::cout << clientData.readString << std::endl;
}

void HttpRequest::parseRequestHeader(clientState &clientData, std::string &reqheader, std::string &readBody) {
	std::istringstream ss(reqheader);
	std::string line;
	while (std::getline(ss, line)) {
		// if (line.empty() == true || line[0] == '\r') {
		// 	WARNING("Reached end of headers");
		// 	break ;
		// }
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
	// for(std::map<std::string, std::string>::iterator it = clientData.header.begin(); it != clientData.header.end(); ++it) {
	// 	std::cout << "MAP" << it->first << ":" << it->second << "MAP";
	// }
	std::map<std::string, std::string>::iterator it = clientData.header.find("Content-Length");
	if (it != clientData.header.end()) {
		try {
			clientData.contentLength = std::atoi(it->second.c_str());
		} catch (const std::invalid_argument& e) {
			clientData.contentLength = 0;
		}
		DEBUG(clientData.contentLength);
		if (clientData.method == POST) {
			clientData.contentType = clientData.header["Content-Type"];
			if (clientData.contentType.empty() == true || clientData.contentType.find("multipart/form-data") == std::string::npos)
				WARNING("Content Type is Empty");
			// DEBUG(readBody);
			parseMultipartFormData(clientData, readBody);
			// parseRequestBody(clientData);
		} else {
			// clientData.bodyString += clientData.readString;
			clientData.bodyString.assign(readBody);
			clientData.flagBodyRead = true;
		}
	} else {
		clientData.contentLength = 0;
	}
}

std::string	HttpRequest::getBoundary(clientState &clientData, std::string const &contentType) {
	size_t	pos = contentType.find("boundary=");
	if (pos != std::string::npos) {
		int i = pos + 9;
		while (contentType[i] == '-')
			i++;
		while (contentType[i] != '\r' && contentType[i + 1] != '\n')
			clientData.boundary += contentType[i++];
	} else
		WARNING("THE BOUNDARY IS MISSING");
	return (clientData.boundary);
}

void HttpRequest::parseMultipartFormData(clientState &clientData, std::string &readBody) {
	std::string	temp;
	size_t i = 0;
	while (i < readBody.size()){
		temp += readBody[i];
		i++;
	}
	DEBUG(temp.size());
	// DEBUG(temp);
	if (clientData.contentLength <= static_cast<ssize_t>(readBody.size())) {
		clientData.flagBodyRead = true;
		size_t	start = temp.find('\n') + 1;
		size_t	end = temp.rfind(clientData.boundary);
		while (temp[end] != '\n')
			end--;
		// clientData.bodyString += temp.substr(start, end - 1);
		clientData.bodyString.assign(temp.substr(start, end - 1));
	}
}