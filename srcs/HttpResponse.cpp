/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:08:24 by otuyishi          #+#    #+#             */
/*   Updated: 2024/07/27 10:21:13 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::generateHtml(int code, const std::string& codeMessage) {
	std::ostringstream stream;

	std::string code_str;
	std::stringstream ss;
	ss << code;
	ss >> code_str;

	stream << "<!DOCTYPE html>"
		<< "<html lang=\"en\">"
		<< "<head>"
		<< "<meta charset=\"UTF-8\">"
		<< "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		<< "<title>Webserv - " << code_str << "</title>"
		<< "<link rel=\"stylesheet\" href=\"\\styles.css\">"
		<< "<link rel=\"icon\" type=\"image/x-icon\" href=\"favicon.ico\">"
		<< "</head>"
		<< "<body class=\"background\">"
		<< "<div class=\"error\">" << code_str << " - " << codeMessage << "</div>"
		<< "<hr>"
		<< "<div class=\"info\"></div>"  // Add specific message if needed
		<< "<button onclick=\"window.history.back()\" class=\"back-button\">Back</button>"
		<< "</body>"
		<< "</html>";

	return stream.str();
}

std::string HttpResponse::statusCode(int code) {
	std::string codeMessage;
	if (code == 400)
		codeMessage = "Bad Request";
	else if (code == 404)
		codeMessage = "Not Found";
	else if (code == 500)
		codeMessage = "Internal Server Error";
	else
		codeMessage = "Unknown Code";
	return generateHtml(code, codeMessage);
}

std::string HttpResponse::metaData(clientState &req) {
	std::string headerMetaData = "";
	std::map<std::string, std::string>::iterator hd = req.header.begin();
	while (hd != req.header.end()) {
		if (hd->first == "Content-Length" || hd->first == "Content-Type" || hd->first == "Connection" || hd->first == "Date" || hd->first == "Server"){
			req.header.erase(hd++);
			// hd = req.header.erase(hd);
		} else {
			headerMetaData += hd->first + ": " + hd->second + "\r\n";
			++hd;
		}
	}
	headerMetaData += "\r\n";
	return headerMetaData;
}

std::string HttpResponse::webserverStamp(void) {
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	return std::string(buf);
}

std::string HttpResponse::errorHandling(int code, clientState &req) {
	std::string errorCode;
	std::stringstream ss;
	ss << code;
	ss >> errorCode;

	std::string sms;
	if (code == 400) sms = "Bad Request";
	else if (code == 404) sms = "Not Found";
	else if (code == 500) sms = "Internal Server Error";
	else sms = "Unknown Error";

	_StatusLine = req.requestLine["httpversion"] + " " + errorCode + " " + sms + "\r\n";

	std::string status_page = statusCode(code);
	std::stringstream pageSizeStream;
	pageSizeStream << status_page.size();
	std::string pageSize;
	pageSizeStream >> pageSize;

	_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize + "\r\nConnection: close\r\n";
	_Body = status_page;
	std::string headerMetaData = metaData(req);
	_Header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_Response = _StatusLine + _Header + _Body;

	return _Response;
}

std::string HttpResponse::respond_Get(clientState &req) {
	std::map<std::string, std::string>::const_iterator url = req.requestLine.find("url");
	if (url != req.requestLine.end()) {
		std::string route = "./www" + (url->second == "/" ? "/index.html" : url->second);
		std::cout << "URL->SECOND: " << url->second << std::endl;
		size_t pos = route.find_last_of('.');
		std::string contentType = g_mimeTypes[route.substr(pos + 1)];
		std::cout << "Content Type: " << contentType << std::endl;
		std::ifstream route_file(route.c_str());
		if (route_file.fail()) {
			return errorHandling(404, req);
		} else {
			std::string buffer((std::istreambuf_iterator<char>(route_file)), std::istreambuf_iterator<char>());
			_StatusLine = req.requestLine["httpversion"] + " 200 OK\r\n";

			std::stringstream ss;
			ss << buffer.size();
			std::string fileSize;
			ss >> fileSize;

			_Header = "Content-Type: " + contentType + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
			_Body = buffer;
			route_file.close();
		}
		std::string headerMetaData = metaData(req);
		_Header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	} else {
		throw std::runtime_error("Url Missing");
	}
	DEBUG("THE RESPONSE HEADER\n" + _Header);
	_Response = _StatusLine + _Header + _Body;
	return _Response;
}
//==================================================================================================

// bool HttpResponse::is_valid_char(char c) {
// 	if (isalnum(c))
// 		return true;
// 	switch (c) {
// 		case '-':
// 		case '_':
// 		case '.':
// 		case '~':
// 			return true;
// 		default:
// 			return false;
// 	}
// }

// bool HttpResponse::is_valid_str(const std::string &str) {
// 	for (size_t i = 0; i == str.length(); ++i) {
// 		if (!is_valid_char(str[i]))
// 			return false;
// 	}
// 	return true;
// }

// std::string HttpResponse::response_Post(clientState &req) {
// 	std::map<std::string, std::string>::const_iterator url = req.requestLine.find("url");
// 	if (url != req.requestLine.end()) {
// 		std::string route = "./www";
// 		if (url->second == "/")
// 			route += "/index.html";
// 		else
// 			route += url->second;
// 		std::ifstream route_file(route.c_str());
// 		std::string extension = url->second;
// 		size_t pos = extension.find_last_of('.');
// 		// std::string mime = parser.mimes[extension.substr(pos + 1)];
// 		if (is_valid_str(route) == false) {
// 			errorHandling(400, req);
// 		// _StatusLine = req.requestLine.find("httpversion")->second + " " + "400" +
// 		// 				" Bad Request\r\n";

// 		// std::string status_page = statusCode(400);
// 		// std::stringstream ss;
// 		// ss << status_page.size();
// 		// std::string pageSize;
// 		// ss >> pageSize;
// 		// _Header = "Content-Type: text/html\r\nContent-Length: " + pageSize +
// 		// 			"\r\nConnection: close\r\n";
// 		// _Body = status_page;
		
// 		} else if (route_file.fail() == true) {
// 			std::ofstream outfile(route);
// 			outfile << req.body;
// 			outfile.close();

// 			std::ifstream test_created_file(route.c_str());
// 			if (test_created_file.fail() == true) {
// 				DEBUG("File failed to create");
// 				throw std::runtime_error("FILE NOT CREATED");
// 			}
// 			std::string buffer;
// 			std::stringstream sss;
// 			sss << test_created_file.rdbuf();
// 			sss >> buffer;
// 			test_created_file.close();
	
// 			_StatusLine = req.requestLine.find("httpversion")->second + " " + "201" + " Created\r\n";
// 			std::stringstream ss;
// 			ss << buffer.size();
// 			std::string fileSize;
// 			ss >> fileSize;
// 			_Header = "Content-Type: " + extension.substr(pos + 1) + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
// 			_Body = buffer;
// 		} else if (route_file.fail() == false) {
// 			_StatusLine = req.requestLine.find("httpversion")->second + " " + "200" + " OK\r\n";
// 			std::string buf;
// 			std::stringstream ss;
// 			ss << route_file.rdbuf();
// 			ss >> buf;
// 			route_file.close();
// 			std::stringstream sss;
// 			sss << buf.size();
// 			std::string fileSize;
// 			sss >> fileSize;
// 			_Header = "Content-Type: " + extension.substr(pos + 1) + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
// 			_Body = buf;
// 		} else {
// 			_StatusLine = req.requestLine.find("httpversion")->second + " " + "500" + " Internal Server Error\r\n";
// 			std::string status_page = statusCode(500);
// 			std::stringstream ss;
// 			ss << status_page.size();
// 			std::string pageSize;
// 			ss >> pageSize;
// 			_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize + "\r\nConnection: close\r\n";
// 			_Body = status_page;
// 		}
// 		time_t now = time(0);
// 		char buf[100];
// 		strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
// 		_Header = _Header + "Date: " + std::string(buf) + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n\r\n";
// 	} else {
// 		throw std::runtime_error("Url Missing");
// 	}
// 	_Response = _StatusLine + _Header + _Body;
// 	DEBUG(_Response);
// 	return (_Response);
// }
//==================================================================================================
bool HttpResponse::is_valid_char(char c) {
	return isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '/';
}

bool HttpResponse::is_valid_str(const std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (!is_valid_char(str[i]))
			return false;
	}
	return true;
}

std::string HttpResponse::response_Post(clientState &req) {
	std::map<std::string, std::string>::const_iterator url = req.requestLine.find("url");
	if (url == req.requestLine.end()) {
		throw std::runtime_error("Url Missing");
	}

	std::string route = "./www" + (url->second == "/" ? "/index.html" : url->second);

	// std::string extension = url->second.substr(url->second.find_last_of('.') + 1);
	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];

	if (!is_valid_str(route)) {
		return errorHandling(400, req);
	}

	std::ifstream route_file(route.c_str());
	if (route_file.fail()) {
		std::ofstream outfile(route.c_str());
		outfile << req.body;
		outfile.close();

		std::ifstream test_created_file(route.c_str());
		if (test_created_file.fail()) {
			DEBUG("File failed to create");
			throw std::runtime_error("FILE NOT CREATED");
		}

		std::string buffer((std::istreambuf_iterator<char>(test_created_file)), std::istreambuf_iterator<char>());
		test_created_file.close();

		_StatusLine = req.requestLine["httpversion"] + " 201 Created\r\n";
		std::stringstream ss;
		ss << buffer.size();
		std::string fileSize;
		ss >> fileSize;
		// _Header = "Content-Type: " + extension + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
		_Header = "Content-Type: " + contentType + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
		_Body = buffer;
	} else {
		std::string buffer((std::istreambuf_iterator<char>(route_file)), std::istreambuf_iterator<char>());
		route_file.close();

		_StatusLine = req.requestLine["httpversion"] + " 200 OK\r\n";
		std::stringstream ss;
		ss << buffer.size();
		std::string fileSize;
		ss >> fileSize;
		// _Header = "Content-Type: " + extension + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
		_Header = "Content-Type: " + contentType + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
		_Body = buffer;
	}

	_Header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + metaData(req);
	_Response = _StatusLine + _Header + _Body;

	// DEBUG(_Response);
	return _Response;
}

//==================================================================================================


// std::string delete_body_sms(int code) {
// 	std::string two_00 = "{\"message\": \"Request deleted successfully\",\"userId\": 123}";
// 	std::string four_04 = "{\"error\": \"Not Found\",\"message\": \"User not found\"}";
// 	std::string five_00 = "{\"error\": \"Internal Server Error\",\"message\": " "\"An unexpected error occurred.\"}";

// 	if (code == 200)
// 		return (two_00);
// 	else if (code == 404)
// 		return (four_04);
// 	else if (code == 500)
// 		return (five_00);
// 	DEBUG(code);
// 	return ("What type of code");
// }

// std::string HttpResponse::response_Delete(clientState &req) {
// 	std::map<std::string, std::string>::const_iterator url = req.requestLine.find("url");
// 	if (url != req.requestLine.end()) {
// 		std::string route = "./www/store/general/" + url->second;
// 		std::ifstream route_file(route.c_str());
// 		std::string extension = url->second;
// 		size_t pos = extension.find_last_of('.');
// 		// std::string mime = parser.mimes[extension.substr(pos + 1)];
// 		if (is_valid_str(route) == false) {
// 			_StatusLine = req.requestLine.find("httpversion")->second + " " + "400" + " Bad Request\r\n";
// 			std::string status_page = statusCode(400);
// 			std::stringstream ss;
// 			ss << status_page.size();
// 			std::string pageSize;
// 			ss >> pageSize;
// 			_Header = "Content-Type: text/html\r\nContent-Length: " + pageSize + "\r\nConnection: close\r\n";
// 			_Body = status_page;
// 		} else if (route_file.fail() == false) {
// 			int content_len;
// 			std::stringstream ss;
// 			ss << req.header.find("Content-Length")->second;
// 			ss >> content_len;
// 			if (content_len == 0 && std::remove(route.c_str()) == 0) {
// 				_StatusLine = req.requestLine.find("httpversion")->second + " " + "204" + " No Content\r\n";
// 				_Header = "Connection: keep-alive\r\n";
// 			} else if (std::remove(route.c_str()) == 0) {
// 				std::string buf = delete_body_sms(200);
// 				std::stringstream ss;
// 				ss >> buf;
// 				route_file.close();
// 				std::stringstream sss;
// 				sss << buf.size();
// 				std::string fileSize;
// 				sss >> fileSize;
// 				_StatusLine = req.requestLine.find("httpversion")->second + " " + "200" + " OK\r\n";
// 				_Header = "Content-Type: " + extension.substr(pos + 1) + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
// 				_Body = buf;
// 			} else {
// 				std::string buf = delete_body_sms(500);
// 				std::stringstream ss;
// 				ss >> buf;
// 				route_file.close();
// 				std::stringstream sss;
// 				sss << buf.size();
// 				std::string fileSize;
// 				sss >> fileSize;
// 				_StatusLine = req.requestLine.find("httpversion")->second + " " + "500" + " Internal Server Error\r\n";
// 				_Header = "Content-Type: " + extension.substr(pos + 1) + "\r\nContent-Length: " + fileSize + "\r\nConnection: close\r\n";
// 				_Body = buf;
// 			}
// 		} else {
// 			std::string buf = delete_body_sms(404);
// 			std::stringstream ss;
// 			ss >> buf;
// 			route_file.close();
// 			std::stringstream sss;
// 			sss << buf.size();
// 			std::string fileSize;
// 			sss >> fileSize;
// 			_StatusLine = req.requestLine.find("httpversion")->second + " " + "404" + " Not Found\r\n";
// 			_Header = "Content-Type: " + extension.substr(pos + 1) + "\r\nContent-Length: " + fileSize + "\r\nConnection: close\r\n";
// 			_Body = buf;
// 		}
// 		time_t now = time(0);
// 		char buf[100];
// 		strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
// 		_Header = _Header + "Date: " + std::string(buf) + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n\r\n";
// 	} else {
// 		throw std::runtime_error("Url Missing");
// 	}
// 	_Response = _StatusLine + _Header + _Body;
// 	return (_Response);
// }
//==================================================================================================

std::string delete_body_sms(int code) {
	if (code == 200)
		return "{\"message\": \"Request deleted successfully\",\"userId\": 123}";
	else if (code == 404)
		return "{\"error\": \"Not Found\",\"message\": \"User not found\"}";
	else if (code == 500)
		return "{\"error\": \"Internal Server Error\",\"message\": \"An unexpected error occurred.\"}";
	
	// DEBUG(code);
	return "What type of code";
}

std::string generateStatusLine(const std::string &httpVersion, int code, const std::string &message) {
	std::ostringstream oss;
	oss << httpVersion << " " << code << " " << message << "\r\n";
	return oss.str();
}

std::string generateHeaders(const std::string &contentType, size_t contentLength, const std::string &connection) {
	std::ostringstream oss;
	oss << "Content-Type: " << contentType << "\r\n"
		<< "Content-Length: " << contentLength << "\r\n"
		<< "Connection: " << connection << "\r\n";
	return oss.str();
}

std::string generateDateHeader() {
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	return std::string("Date: ") + buf + "\r\n";
}

std::string HttpResponse::response_Delete(clientState &req) {
	std::map<std::string, std::string>::const_iterator url = req.requestLine.find("url");
	if (url == req.requestLine.end()) {
		throw std::runtime_error("Url Missing");
	}

	std::string route = "./www/store/general/" + url->second;
	std::ifstream route_file(route.c_str());
	// std::string extension = url->second;
	size_t pos = route.find_last_of('.');
	// std::string contentType = extension.substr(pos + 1);
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];

	if (!is_valid_str(route)) {
		_StatusLine = generateStatusLine(req.requestLine.find("httpversion")->second, 400, "Bad Request");
		std::string status_page = statusCode(400);
		_Header = generateHeaders("text/html", status_page.size(), "close");
		_Body = status_page;
	} else if (!route_file.fail()) {
		int content_len;
		std::istringstream(req.header.find("Content-Length")->second) >> content_len;

		if (content_len == 0 && std::remove(route.c_str()) == 0) {
			_StatusLine = generateStatusLine(req.requestLine.find("httpversion")->second, 204, "No Content");
			_Header = generateHeaders("", 0, "keep-alive");
		} else if (std::remove(route.c_str()) == 0) {
			std::string buf = delete_body_sms(200);
			_StatusLine = generateStatusLine(req.requestLine.find("httpversion")->second, 200, "OK");
			_Header = generateHeaders(contentType, buf.size(), "keep-alive");
			_Body = buf;
		} else {
			std::string buf = delete_body_sms(500);
			_StatusLine = generateStatusLine(req.requestLine.find("httpversion")->second, 500, "Internal Server Error");
			_Header = generateHeaders(contentType, buf.size(), "close");
			_Body = buf;
		}
	} else {
		std::string buf = delete_body_sms(404);
		_StatusLine = generateStatusLine(req.requestLine.find("httpversion")->second, 404, "Not Found");
		_Header = generateHeaders(contentType, buf.size(), "close");
		_Body = buf;
	}

	_Header += generateDateHeader() + "Server: Webserv/harsh/oreste/v1.0\r\n\r\n";
	_Response = _StatusLine + _Header + _Body;

	// DEBUG(_Response);
	return _Response;
}

//==================================================================================================
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
