/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/18 12:45:28 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"


HttpRequest::HttpRequest(){}

HttpRequest::~HttpRequest(){}

//Request Line
//Header
//Body-->optional
void HttpRequest::request_blocks(std::string request) {
	std::string::size_type blocks_pos = request.find("\r\n\r\n");
	if (blocks_pos == std::string::npos)
		throw std::runtime_error("Request must contain at least a method and header");

	std::string::size_type req_line_pos = request.find("\r\n");
	if (req_line_pos == std::string::npos)
		throw std::runtime_error("Invalid request format");

	std::string request_line = request.substr(0, req_line_pos);
	receive_request_line(request_line);

	std::string header = request.substr(req_line_pos + 2, blocks_pos - (req_line_pos + 2));
	receive_request_header(header);
}

// POST /submit-form HTTP/1.1\r\n
//METHOD SP URI SP HTTP-VERSION CRLF
void	HttpRequest::receive_request_line(std::string line) {
	std::stringstream ss(line);
	while (!ss.eof()) {
		std::string	method;
		ss>>method;
		ReqLine.insert({"method", method});

		std::string	url;
		ss>>url;
		ReqLine.insert({"url", url});

		std::string	httpversion;
		ss>>httpversion;
		ReqLine.insert({"httpversion", httpversion});
	}
}

// Host: www.example.com\r\n
// Content-Type: application/x-www-form-urlencoded\r\n
// Content-Length: 27\r\n
// \r\n
void	HttpRequest::receive_request_header(std::string header) {
	size_t pos = 0;
	std::string token;
	std::string delimit1 = "\r\n";

	while ((pos = header.find(delimit1)) != std::string::npos) {
		token = header.substr(0, pos);
		size_t colon = 0;
		std::map <std::string, std::string> head_content;
		std::string key;
		std::string value;
		std::string delimit2 = ":";
		while ((colon = token.find(delimit2)) != std::string::npos) {
			key = token.substr(0, colon);
			if (key == "Content-Length") {
				int	len = 0;
				std::istringstream(token.substr(colon + 1, colon + delimit2.length())) >> len;
				_ContentLen = len;
				header.erase(0, pos + delimit1.length());
				break ;
			}
			value = token.substr(colon + 1, colon + delimit2.length());
			head_content.insert({key, value});
		}
		_Header.push_back(head_content);
		header.erase(0, pos + delimit1.length());
	}
}






























//My Notes
//Requests
//-->GET<--
//purpose: retrieves data, parameters in URL, no body
//mandatory method('GET'), \
			path('/search?q=example'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->POST<--
//purpose: sends data to server, parameters in body, creates/updates resources
//mandatory method('POST'), \
			path('/submit-form'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type), \
			end of headers('\r\n\r\n') \
			body('name=John+Doe&age=30&city=NY')

//-->DELETE<--
//requests resource deletion, no body
//mandatory method('DELETE'), \
			path('/delete-profile/123'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

// POST /submit-form HTTP/1.1\r\n
// Host: www.example.com\r\n
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\n
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
// Accept-Language: en-US,en;q=0.5\r\n
// Accept-Encoding: gzip, deflate, br\r\n
// Content-Type: application/x-www-form-urlencoded\r\n
// Content-Length: 27\r\n
// Connection: keep-alive\r\n
// Upgrade-Insecure-Requests: 1\r\n
// \r\n
// key1=value1&key2=value2




//parse end of buffer

//requestLine
//-->vector["POST", "/submit-form", "HTTP/1.1"]======>till '''\r\n'''

//Header
//map{
//		User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\n
// 		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
// 		Accept-Language: en-US,en;q=0.5\r\n
// 		Accept-Encoding: gzip, deflate, br\r\n
// 		Content-Type: application/x-www-form-urlencoded\r\n
// 		Content-Length: 27\r\n
// 		Connection: keep-alive\r\n
// 		Upgrade-Insecure-Requests: 1\r\n
// }-->fromatted as Key:Value, key up to ':' and value up to '\r\n'
//=======> header ends with '\r\n\r\n'

//Body
//string
