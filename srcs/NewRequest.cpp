/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:24:49 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/05 16:29:30 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NewRequest.hpp"

NewRequest::NewRequest() {};

NewRequest::~NewRequest() {};

std::string::size_type NewRequest::findDelimiter(clientState &state, const std::string& delimiter, std::string::size_type start) {
	return state.readString.find(delimiter, start);
}

std::map<std::string, std::string>	NewRequest::parameterizeRequestLine(std::string	rLine, clientState &state) {
	std::stringstream ss(rLine);
	std::string value;

	if (ss >> value)
		state.requestLine["method"] = value;
	if (ss >> value)
		state.requestLine["url"] = value;
	if (ss >> value)
		state.requestLine["httpversion"] = value;
	return state.requestLine;
}

std::string	NewRequest::trim(const std::string& str) {
	const char* whitespace = " \t\n\r\f\v";
	std::string::size_type first = str.find_first_not_of(whitespace);
	if (first == std::string::npos)
		return "";
	std::string::size_type last = str.find_last_not_of(whitespace);
	return std::string(str.c_str() + first, last - first + 1);
}

std::map<std::string, std::string> NewRequest::parameterizeHeader(const std::string& headerStr, clientState &state) {
	std::map<std::string, std::string> header;
	std::string::size_type start = 0;
	std::string::size_type end = findDelimiter(state, "\r\n", start);
	state.contentLenPresent = false;
	state.contentLength = 0;

	while (end != std::string::npos) {
		std::string::size_type delimiterPos = findDelimiter(state, ": ", start);
		if (delimiterPos != std::string::npos && delimiterPos < end) {
			std::string key(headerStr.c_str() + start, delimiterPos - start);
			std::string value(headerStr.c_str() + delimiterPos + 2, end - delimiterPos - 2);
			key = trim(key);
			value = trim(value);
			header[key] = value;
			if (key == "Content-Length") {
				state.contentLenPresent = true;
				state.contentLength = atoi(value.c_str());	
			}
		}
		start = end + 2;
		end = findDelimiter(state, "\r\n", start);
	}
	if (start < headerStr.length()) {
		std::string::size_type delimiterPos = findDelimiter(state, ": ", start);
		if (delimiterPos != std::string::npos) {
			std::string key(headerStr.c_str() + start, delimiterPos - start);
			std::string value(headerStr.c_str() + delimiterPos + 2, headerStr.length() - delimiterPos - 2);
			key = trim(key);
			value = trim(value);
			header[key] = value;
			if (key == "Content-Length") {
				state.contentLenPresent = true;
				state.contentLength = atoi(value.c_str());
			}
		}
	}
	state.flagHeaderRead = true;
	return header;
}

std::vector<std::string> NewRequest::chunkRequest(clientState &state) {
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end;

	end = state.readString.find("\r\n", start);
	if (end != std::string::npos) {
		result.push_back(std::string(state.readString.c_str() + start, end - start));
		start = end + 2;
	}
	end = findDelimiter(state, "\r\n\r\n", start);
	if (end != std::string::npos) {
		result.push_back(std::string(state.readString.c_str() + start, end - start));
		start = end + 4;
	}
	if (start < state.readString.size())
		result.push_back(std::string(state.readString.c_str() + start, state.readString.size() - start));
	// std::cout << result[0] << std::endl;
	// std::cout << result[1] << std::endl;
	// std::cout << result[3] << std::endl;
	return (result);
}

//NOTEs
//handling application/x-www-form-urlencoded
//mapped up together as we construct the body response required
//urlDecode decodes the original string received and then
//example of this case: "name=John+Doe&age=31&city=New+York"
std::string urlDecode(const std::string &str) {
	std::string result;
	result.reserve(str.size());

	size_t i = 0;
	while (i < str.size()) {
		if (str[i] == '%' && i + 2 < str.size() &&
			std::isxdigit(static_cast<unsigned char>(str[i + 1])) && std::isxdigit(static_cast<unsigned char>(str[i + 2]))) {
			int value;
			std::sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
			result += static_cast<char>(value);
			i += 3;
		} else if (str[i] == '+') {
			result += ' ';
			++i;
		} else {
			result += str[i];
			++i;
		}
	}
	return result;
}

void extractKeyValuePair(const std::string &body, size_t start, size_t end, std::string &responseBody) {
	size_t eqPos = body.find('=', start);
	if (eqPos != std::string::npos && eqPos < end) {
		std::string key, value;
		for (size_t i = start; i < eqPos; ++i)
			key += body[i];
		for (size_t i = eqPos + 1; i < end; ++i)
			value += body[i];
		key = urlDecode(key);
		value = urlDecode(value);
		// parsedData[key] = value;
		responseBody += "Key: " + key + ", Value: " + value + "\n";
	}
}

std::string parseAppFormUrlencoded(std::string body) {
	std::string responseBody = "Parsing completed\n";

	size_t start = 0;
	size_t end = body.find('&');
	while (end != std::string::npos) {
		extractKeyValuePair(body, start, end, responseBody);
		start = end + 1;
		end = body.find('&', start);
	}
	extractKeyValuePair(body, start, body.size(), responseBody);
	return responseBody;
}

//let's handle multipart/form-data
//example:
// "Content-Type: multipart/form-data; boundary=123456789\n\n"
//                  "--123456789\n"
//                  "Content-Disposition: form-data; name=\"field1\"\n\n"
//                  "value1\n"
//                  "--123456789\n"
//                  "Content-Disposition: form-data; name=\"field2\"\n\n"
//                  "value2\n"
//                  "--123456789--";

std::string	getBoundary(std::string const &contentType) {
	size_t	pos = contentType.find("boundary=");
	std::string	boundary = "";
	if (pos != std::string::npos) {
		int i = pos + 9;
		while (contentType[i] == '-')
			i++;
		while (contentType[i] != '\r' && contentType[i + 1] != '\n')
			boundary += contentType[i++];
	} else
		std::cerr << "THE BOUNDARY IS MISSING" << std::endl;
		// WARNING("THE BOUNDARY IS MISSING");
	return (boundary);
}

std::string parseMultipartFormData(std::string contentType, std::string bodyStr) {
	std::string boundary = getBoundary(contentType);
	size_t	posBodyEnd = bodyStr.rfind(boundary);
	size_t	firstLinePos = bodyStr.find('\n');
	size_t j = posBodyEnd;
	while (bodyStr[j] != '\n')
		j--;
	size_t i = firstLinePos + 1;
	std::string theBody;
	while (i <= j){
		theBody += bodyStr[i];
		i++;
	}
	// std::cout << theBody << std::endl;
	return (theBody);
}

//application json
//for example: 
std::string	parseApplicationJson(clientState &state) {
	return (state.bodyString);
}

std::string	requestedBodyParsing(clientState &state, std::string bodyStr) {
	std::map<std::string, std::string>::iterator it = state.header.find("Content-Type");
	if (it != state.header.end()) {
		const std::string &contentType = it->second;
		if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
			state.bodyString = parseAppFormUrlencoded(bodyStr);
		else if (contentType.find("multipart/form-data") != std::string::npos)
			state.bodyString = parseMultipartFormData(it->second, bodyStr);
		else if (contentType.find("application/json") != std::string::npos)
			state.bodyString = parseApplicationJson(state);
		else
			state.bodyString = state.bodyString;
	} else
		return "400 BAD REQUEST";//==>response function should be returned here with the 400 error
	return state.bodyString;
}

void NewRequest::requestBlock(clientState &state) {
	if (state.readString.empty() == true) {
		WARNING("Recieved empty request");
		return;
	}

	std::vector<std::string> splittedRequest = chunkRequest(state);
	std::string rLine = splittedRequest[0];
	// DEBUG(rLine);
	// std::map<std::string, std::string> requestLine = parameterizeRequestLine(rLine);
	state.requestLine = parameterizeRequestLine(rLine, state);
	// std::cout << rLine << std::endl;
	// for (std::map<std::string, std::string>::iterator it = requestLine.begin(); it != requestLine.end(); ++it) {
	// 	std::cout << it->first << ": " << it->second << "\n==================================================\n" << std::endl;
	// }
	
	std::string head = splittedRequest[1];
	std::map<std::string, std::string> splittedHeader = parameterizeHeader(head, state);
	state.header = parameterizeHeader(head, state);
	// std::cout << header << std::endl;
	for (std::map<std::string, std::string>::iterator it = splittedHeader.begin(); it != splittedHeader.end(); ++it) {
		std::cout << it->first << ":" << it->second << "\n==================================================\n" << std::endl;
	}

	if (state.contentLength > 0) {
		if (state.requestLine["method"] == "POST")
			state.bodyString = requestedBodyParsing(state, splittedRequest[2]);
		else
			state.bodyString = splittedRequest[2];
	}
	state.flagBodyRead = true;
	// std::cout << state.bodyString << std::endl;
}


// int main() {
// 	std::string str = "POST /upload/ HTTP/1.1\r\n"
// 					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
// 					"Host: localhost:8000\r\n"
// 					"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36\r\n"
// 					"Accept-Encoding: gzip, deflate, br, zstd\r\n"
// 					"Accept-Language: en-US,en;q=0.9\r\n"
// 					"Cache-Control: max-age=0\r\n"
// 					"Referer: https://didactic-space-happiness-4xw76796wfqjr9-8000.app.github.dev/\r\n"
// 					"X-Request-ID: df056025d4eac639bb89b090b25b3c8a\r\n"
// 					"X-Real-IP: 10.240.0.69\r\n"
// 					"X-Forwarded-Port: 443\r\n"
// 					"X-Forwarded-Scheme: https\r\n"
// 					"X-Original-URI: /upload/\r\n"
// 					"X-Scheme: https\r\n"
// 					"sec-ch-ua: \"Not/A)Brand\";v=\"8\", \"Chromium\";v=\"126\", \"Google Chrome\";v=\"126\"\r\n"
// 					"sec-ch-ua-mobile: ?0\r\n"
// 					"sec-ch-ua-platform: \"macOS\"\r\n"
// 					"sec-fetch-site: same-origin\r\n"
// 					"sec-fetch-mode: navigate\r\n"
// 					"sec-fetch-user: ?1\r\n"
// 					"sec-fetch-dest: document\r\n"
// 					"priority: u=0, i\r\n"
// 					"X-Original-Proto: https\r\n"
// 					"X-Forwarded-Proto: https\r\n"
// 					"X-Forwarded-Host: didactic-space-happiness-4xw76796wfqjr9-8000.app.github.dev\r\n"
// 					"X-Forwarded-For: 10.240.0.69\r\n"
// 					"Origin: http://localhost:8000\r\n"
// 					"Proxy-Connection: Keep-Alive\r\n"
// 					"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryQMvXh7LsqCHQEiiN\r\n"
// 					"Content-Length: 687\r\n"
// 					"\r\n"
// 					"------WebKitFormBoundaryQMvXh7LsqCHQEiiN\n"
// 					"Content-Disposition: form-data; name=\"file\"; filename=\"Makefile\"\n"
// 					"Content-Type: application/octet-stream\n"
// 					"\n"
// 					"NAME = RPN\n"
// 					"\n"
// 					"CC = c++\n"
// 					"\n"
// 					"FLAGS = -Wall -Wextra -Werror -std=c++98\n"
// 					"\n"
// 					"RM = rm -f\n"
// 					"\n"
// 					"SRCS = main.cpp RPN.cpp\n"
// 					"\n"
// 					"OBJDIR = ./Objs\n"
// 					"\n"
// 					"OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)\n"
// 					"\n"
// 					"INCLUDES = -I./includes\n"
// 					"\n"
// 					"all: $(NAME)\n"
// 					"\n"
// 					"$(NAME): $(OBJS)\n"
// 					"        $(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)\n"
// 					"\n"
// 					"$(OBJDIR)/%.o: %.cpp\n"
// 					"        @mkdir -p $(OBJDIR)/src\n"
// 					"        $(CC) $(FLAGS) $(INCLUDES) -c $< -o $@\n"
// 					"\n"
// 					"clean:\n"
// 					"                $(RM) $(OBJDIR)/*.o $(OBJDIR)/src/*.o\n"
// 					"                @rm -rf $(OBJDIR)\n"
// 					"\n"
// 					"fclean: clean\n"
// 					"                $(RM) $(NAME)\n"
// 					"\n"
// 					"re: fclean all\n"
// 					"\n"
// 					".PHONY: all clean fclean re\n"
// 					"\n"
// 					"------WebKitFormBoundaryQMvXh7LsqCHQEiiN--";
	
	// std::vector<std::string> tokens = splitRequestString(str);
	// for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
	// 	std::cout << "\n\nToken: \n"" << *it << "\"" << std::endl;
	// }

	// std::vector<std::string> tokens = splitRequestString(str);
	// std::cout << "\nREQUESTLINE\n======================: \n" << tokens[0] << "\n===========================\n" << std::endl;
	// std::cout << "\nHEADER\n======================: \n" << tokens[1]  << "\n===========================\n" << std::endl;
	// std::cout << "\nBODY\n======================: \n" << tokens[2]  << "\n===========================\n" << std::endl;

	// chunkRequest(splitRequestString(str));

	
// 	std::vector<std::string> parts = splitRequestString(str);
// 	std::map<std::string, std::string> headers = parameterizeHeader(chunkRequest(parts));
	
// 	return 0;
// }

// std::string	NewRequest::requesting(clientState request) {
// 	std::string requestLine = split(request, "\r\n");
// }

// Request: POST /upload/ HTTP/1.1\r\n
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n
// Host: localhost:8000\r\n
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36\r\n
// Accept-Encoding: gzip, deflate, br, zstd\r\n
// Accept-Language: en-US,en;q=0.9\r\n
// Cache-Control: max-age=0\r\n
// Referer: https://didactic-space-happiness-4xw76796wfqjr9-8000.app.github.dev/\r\n
// X-Request-ID: df056025d4eac639bb89b090b25b3c8a\r\n
// X-Real-IP: 10.240.0.69\r\n
// X-Forwarded-Port: 443\r\n
// X-Forwarded-Scheme: https\r\n
// X-Original-URI: /upload/\r\n
// X-Scheme: https
// sec-ch-ua: "Not/A)Brand";v="8", "Chromium";v="126", "Google Chrome";v="126"\r\n
// sec-ch-ua-mobile: ?0\r\n
// sec-ch-ua-platform: "macOS"\r\n
// sec-fetch-site: same-origin\r\n
// sec-fetch-mode: navigate\r\n
// sec-fetch-user: ?1\r\n
// sec-fetch-dest: document\r\n
// priority: u=0, i\r\n
// X-Original-Proto: https\r\n
// X-Forwarded-Proto: https\r\n
// X-Forwarded-Host: didactic-space-happiness-4xw76796wfqjr9-8000.app.github.dev\r\n
// X-Forwarded-For: 10.240.0.69\r\n
// Origin: http://localhost:8000\r\n
// Proxy-Connection: Keep-Alive\r\n
// Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryQMvXh7LsqCHQEiiN\r\n
// Content-Length: 687\r\n
// \r\n
// ------WebKitFormBoundaryQMvXh7LsqCHQEiiN
// Content-Disposition: form-data; name="file"; filename="Makefile"
// Content-Type: application/octet-stream

// NAME = RPN

// CC = c++

// FLAGS = -Wall -Wextra -Werror -std=c++98

// RM = rm -f

// SRCS = main.cpp RPN.cpp

// OBJDIR = ./Objs

// OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

// INCLUDES = -I./includes

// all: $(NAME)

// $(NAME): $(OBJS)
//         $(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

// $(OBJDIR)/%.o: %.cpp
//         @mkdir -p $(OBJDIR)/src
//         $(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

// clean:
//                 $(RM) $(OBJDIR)/*.o $(OBJDIR)/src/*.o
//                 @rm -rf $(OBJDIR)

// fclean: clean
//                 $(RM) $(NAME)

// re: fclean all

// .PHONY: all clean fclean re

// ------WebKitFormBoundaryQMvXh7LsqCHQEiiN--