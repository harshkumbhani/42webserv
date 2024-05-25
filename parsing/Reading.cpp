/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reading.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/22 11:46:18 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reading.hpp"

//construction
Reading::Reading(){}
//destruction
Reading::~Reading(){}

//getters
std::string	Reading::getBuffer(void) const {
	return (this->_Buffer);
}

std::string	Reading::getHttpMethod(void) const {
	return (this->_HttpMethod);
}

std::string	Reading::getUrl(void) const {
	return (this->_Url);
}

std::string	Reading::getHttpVers(void) const {
	return (this->_HttpVers);
}

std::string	Reading::getHeadStorage(void) const {
	return (this->_HeadStorage);
}

std::string	Reading::getHost(void) const {
	return (this->_Host);
}

std::string	Reading::getUserAgent(void) const {
	return (this->_UserAgent);
}

std::string	Reading::getAccept(void) const {
	return (this->_Accept);
}

std::string	Reading::getBody(void) const {
	return (this->_Body);
}

std::string	Reading::getCurrentLine(void) const {
	return (this->_CurrentLine);
}

int			Reading::getIndex(void) const {
	return (this->_Index);
}

int			Reading::getLen(void) const {
	return (this->_Len);
}

bool		Reading::getFlag(void) const {
	return (this->_Flag);
}


//setters
void	Reading::setBuffer(std::string buffer) {
	this->_Buffer = buffer;
}

void	Reading::setHttpMethod(std::string httpMethod) {
	this->_HttpMethod = httpMethod;
}

void	Reading::setUrl(std::string url) {
	this->_Url = url;
}

void	Reading::setHttpVers(std::string httpVers) {
	this->_HttpVers = httpVers;
}

void	Reading::setHeadStorage(std::string headStorage) {
	this->_HeadStorage = headStorage;
}

void	Reading::setHost(std::string host) {
	this->_Host = host;
}

void	Reading::setUserAgent(std::string userAgent) {
	this->_UserAgent = userAgent;
}

void	Reading::setAccept(std::string accept) {
	this->_Accept = accept;
}

void	Reading::setBody(std::string body) {
	this->_Body = body;
}

void	Reading::setCurrentLine(std::string currentLine) {
	this->_CurrentLine = currentLine;
}

void	Reading::setIndex(int index) {
	this->_Index = index;
}

void	Reading::setLen(int len) {
	this->_Len = len;
}

void	Reading::setFlag(bool flag) {
	this->_Flag = flag;
}


//Functions
void	Reading::trimming(std::string str) {
	;
}

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

//-->PUT<--
//purpose: sends full data to server, parameters in body, creates/updates resources
//mandatory method('PUT'), \
			path('/update-profile'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type), \
			end of headers('\r\n\r\n')

//-->DELETE<--
//requests resource deletion, no body
//mandatory method('DELETE'), \
			path('/delete-profile/123'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->HEAD<--
//purpose: retrieves headers, no body
//mandatory method('HEAD'), \
			path('/check-resource'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->OPTIONs<--
//purpose: Describes communication options, no body
//mandatory method('OPTIONs'), \
			path('/api/resource'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->PATCH<--
//purpose: sends partial data to update a resource, parameters in body
//mandatory method('PATCH'), \
			path('/update-profile'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type) \
			end of headers('\r\n\r\n') \
			Body({"email":"email@example.com"})

//-->CONNECT<--
//purpose: establishes a tunnel, no typical body in inial request
//mandatory method('CONNECT'), \
			Host and port('example.com:443'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent, 'Proxy-Authorization')
