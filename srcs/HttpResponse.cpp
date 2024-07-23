/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:08:24 by otuyishi          #+#    #+#             */
/*   Updated: 2024/07/23 12:17:38 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::statusCode(int code) {
std::string code_str;
std::stringstream ss(code);
ss >> code_str;
std::string part1 =
	"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>" +
	code_str + "</title></head><body><h1>";
std::string part2 = "</h1></body></html>";
if (code == 400)
	return (part1 + " Bad Request" + part2);
else if (code == 404)
	return (part1 + " Not Found" + part2);
else if (code == 500)
	return (part1 + " Internal Server Error" + part2);
return ("NULL");
}

std::string HttpResponse::respond_Get(clientState &req) {
std::map<std::string, std::string>::const_iterator url =
	req.requestLine.find("url");
if (url != req.requestLine.end()) {
	std::string route = "./www";
	if (url->second == "/")
		route += "/index.html";
	else
		route += url->second;
	std::ifstream route_file(route.c_str());
	if (route_file.fail() == true) {
	_StatusLine = req.requestLine.find("httpversion")->second + " " + "404" +
					" Not Found\r\n";
	_Header = "Content-Type: text/html\r\n";
	std::string status_page = statusCode(404);
	std::stringstream ss;
	ss << status_page.size();
	std::string pageSize;
	ss >> pageSize;

	_Header =
		_Header + "Content-Length: " + pageSize + "\r\nConnection: close\r\n";
	_Body = status_page;
	} else {
	std::string buffer;
	buffer.assign(std::istreambuf_iterator<char>(route_file),
					std::istreambuf_iterator<char>());

	_StatusLine =
		req.requestLine.find("httpversion")->second + " " + "200" + " OK\r\n";
	_Header = "Content-Type: text/html\r\n";

	std::stringstream ss;
	ss << buffer.size();
	std::string fileSize;
	ss >> fileSize;

	_Header = _Header + "Content-Length: " + fileSize +
				"\r\nConnection: keep-alive\r\n";
	_Body = buffer;
	route_file.close();
	}
	std::string headerMetaData = "";
	for (std::map<std::string, std::string>::const_iterator hd = req.header.begin(); hd != req.header.end(); hd++) {
		if (hd->first == "Content-Length" || hd->first == "Content-Type" || hd->first == "Connection" || hd->first == "Date" || hd->first == "Server")
			req.header.erase(hd->first);
		else
			headerMetaData = headerMetaData + hd->first + hd->second + "\r\n";
	}
	headerMetaData += "\r\n";
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	_Header = _Header + "Date: " + std::string(buf) +
			"\r\nServer: Webserv/harsh/oreste/v1.0" + headerMetaData;
} else {
	throw std::runtime_error("Url Missing");
}
_Response = _StatusLine + _Header + _Body;
return (_Response);
}

bool HttpResponse::is_valid_char(char c) {
if (isalnum(c))
	return true;
switch (c) {
case '-':
case '_':
case '.':
case '~':
	return true;
default:
	return false;
}
}

bool HttpResponse::is_valid_str(const std::string &str) {
for (size_t i = 0; i == str.length(); ++i) {
	if (!is_valid_char(str[i]))
	return false;
}
return true;
}

// HTTP/1.1 200 OK
// Content-Type: application/json
//  Content-Length: 51
//  Date: Wed, 31 May 2024 12:34:56 GMT
//  Server: MySimpleServer/1.0
//  Connection: keep-alive
//  {
//    "message": "Data processed successfully"
//  }

std::string HttpResponse::response_Post(clientState &req) {
std::map<std::string, std::string>::const_iterator url =
	req.requestLine.find("url");
if (url != req.requestLine.end()) {
	std::string route = "./www" + url->second;
	std::ifstream route_file(route.c_str());
	std::string extension = url->second;
	size_t pos = extension.find_last_of('.');
	if (is_valid_str(route) == false) {
	_StatusLine = req.requestLine.find("httpversion")->second + " " + "400" +
					" Bad Request\r\n";
	// std::string error_page = statusCode(400);
	// page_processing(error_page);
	std::string status_page = statusCode(400);
	std::stringstream ss;
	ss << status_page.size();
	std::string pageSize;
	ss >> pageSize;
	_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize +
				"\r\nConnection: close\r\n";
	_Body = status_page;
	} else if (route_file.fail() == true) {
	std::ofstream outfile(route);
	outfile << req.body;
	outfile.close();

	std::ifstream test_created_file(route.c_str());
	if (test_created_file.fail() == true) {
		DEBUG("File failed to create");
		throw std::runtime_error("FILE NOT CREATED");
	}
	std::string buffer;
	std::stringstream sss;
	sss << test_created_file.rdbuf();
	sss >> buffer;
	test_created_file.close();

	_StatusLine = req.requestLine.find("httpversion")->second + " " + "201" +
					" Created\r\n";
	std::stringstream ss;
	ss << buffer.size();
	std::string fileSize;
	ss >> fileSize;
	_Header = "Content-Type: " + extension.substr(pos + 1) +
				"\r\nContent-Length: " + fileSize +
				"\r\nConnection: keep-alive\r\n";
	_Body = buffer;
	} else if (route_file.fail() == false) {
	_StatusLine =
		req.requestLine.find("httpversion")->second + " " + "200" + " OK\r\n";
	std::string buf;
	std::stringstream ss;
	ss << route_file.rdbuf();
	ss >> buf;
	route_file.close();
	std::stringstream sss;
	sss << buf.size();
	std::string fileSize;
	sss >> fileSize;
	_Header = "Content-Type: " + extension.substr(pos + 1) +
				"\r\nContent-Length: " + fileSize +
				"\r\nConnection: keep-alive\r\n";
	_Body = buf;
	} else {
	_StatusLine = req.requestLine.find("httpversion")->second + " " + "500" +
					" Internal Server Error\r\n";
	std::string status_page = statusCode(500);
	std::stringstream ss;
	ss << status_page.size();
	std::string pageSize;
	ss >> pageSize;
	_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize +
				"\r\nConnection: close\r\n";
	_Body = status_page;
	}
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	_Header = _Header + "Date: " + std::string(buf) +
			"\r\nServer: Webserv/harsh/oreste/v1.0\r\n\r\n";
} else {
	throw std::runtime_error("Url Missing");
}
_Response = _StatusLine + _Header + _Body;
return (_Response);
}

// HTTP/1.1 200 OK
// Content-Type: application/json
// Content-Length: 51
// Date: Mon, 31 May 2024 12:00:00 GMT
// Connection: keep-alive

// {
//   "message": "User deleted successfully",
//   "userId": 123
// }

std::string delete_body_sms(int code) {
std::string two_00 =
	"{\"message\": \"Request deleted successfully\",\"userId\": 123}";
std::string four_04 =
	"{\"error\": \"Not Found\",\"message\": \"User not found\"}";
std::string five_00 = "{\"error\": \"Internal Server Error\",\"message\": "
						"\"An unexpected error occurred.\"}";

if (code == 200)
	return (two_00);
else if (code == 404)
	return (four_04);
else if (code == 500)
	return (five_00);
DEBUG(code);
return ("What type of code");
}

std::string HttpResponse::response_Delete(clientState &req) {
std::map<std::string, std::string>::const_iterator url =
	req.requestLine.find("url");
if (url != req.requestLine.end()) {
	std::string route = "./www/store/general/" + url->second;
	std::ifstream route_file(route.c_str());
	std::string extension = url->second;
	size_t pos = extension.find_last_of('.');
	if (is_valid_str(route) == false) {
	_StatusLine = req.requestLine.find("httpversion")->second + " " + "400" +
					" Bad Request\r\n";
	std::string status_page = statusCode(400);
	std::stringstream ss;
	ss << status_page.size();
	std::string pageSize;
	ss >> pageSize;
	_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize +
				"\r\nConnection: close\r\n";
	_Body = status_page;
	} else if (route_file.fail() == false) {
	int content_len;
	std::stringstream ss;
	ss << req.header.find("Content-Length")->second;
	ss >> content_len;
	if (content_len == 0 && std::remove(route.c_str()) == 0) {
		_StatusLine = req.requestLine.find("httpversion")->second + " " +
					"204" + " No Content\r\n";
		_Header = "Connection: keep-alive\r\n";
	} else if (std::remove(route.c_str()) == 0) {
		std::string buf = delete_body_sms(200);
		std::stringstream ss;
		ss >> buf;
		route_file.close();
		std::stringstream sss;
		sss << buf.size();
		std::string fileSize;
		sss >> fileSize;
		_StatusLine = req.requestLine.find("httpversion")->second + " " +
					"200" + " OK\r\n";
		_Header = "Content-Type: " + extension.substr(pos + 1) +
				"\r\nContent-Length: " + fileSize +
				"\r\nConnection: keep-alive\r\n";
		_Body = buf;
	} else {
		std::string buf = delete_body_sms(500);
		std::stringstream ss;
		ss >> buf;
		route_file.close();
		std::stringstream sss;
		sss << buf.size();
		std::string fileSize;
		sss >> fileSize;
		_StatusLine = req.requestLine.find("httpversion")->second + " " +
					"500" + " Internal Server Error\r\n";
		_Header = "Content-Type: " + extension.substr(pos + 1) +
				"\r\nContent-Length: " + fileSize +
				"\r\nConnection: close\r\n";
		_Body = buf;
	}
	} else {
	std::string buf = delete_body_sms(404);
	std::stringstream ss;
	ss >> buf;
	route_file.close();
	std::stringstream sss;
	sss << buf.size();
	std::string fileSize;
	sss >> fileSize;
	_StatusLine = req.requestLine.find("httpversion")->second + " " + "404" +
					" Not Found\r\n";
	_Header = "Content-Type: " + extension.substr(pos + 1) +
				"\r\nContent-Length: " + fileSize + "\r\nConnection: close\r\n";
	_Body = buf;
	}
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	_Header = _Header + "Date: " + std::string(buf) +
			"\r\nServer: Webserv/harsh/oreste/v1.0\r\n\r\n";
} else {
	throw std::runtime_error("Url Missing");
}
_Response = _StatusLine + _Header + _Body;
return (_Response);
}

std::string HttpResponse::respond(clientState &req) {
	std::map<std::string, std::string>::const_iterator metho =
		req.requestLine.find("method");
	if (metho != req.requestLine.end() && metho->second == "GET")
		respond_Get(req);
	else if (metho != req.requestLine.end() && metho->second == "POST")
		response_Post(req);
	else if (metho != req.requestLine.end() && metho->second == "DELETE")
		response_Delete(req);
	return _Response;
}
