/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reading.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/22 11:39:38 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READING_HPP
# define READING_HPP

#include <iostream>
#include <map>

class Reading {
	public:
		//construction
		Reading();
		//destruction
		~Reading();

		//getters
		std::string	getBuffer(void) const;
		std::string	getHttpMethod(void) const;
		std::string	getUrl(void) const;
		std::string	getHttpVers(void) const;
		std::string	getHeadStorage(void) const;
		std::string	getHost(void) const;
		std::string	getUserAgent(void) const;
		std::string	getAccept(void) const;
		std::string	getBody(void) const;
		std::string	getCurrentLine(void) const;
		int			getIndex(void) const;
		int			getLen(void) const;
		bool		getFlag(void) const;

		//setters
		void	setBuffer(std::string buffer);
		void	setHttpMethod(std::string httpMethod);
		void	setUrl(std::string url);
		void	setHttpVers(std::string httpVers);
		void	setHeadStorage(std::string headStorage);
		void	setHost(std::string host);
		void	setUserAgent(std::string userAgent);
		void	setAccept(std::string accept);
		void	setBody(std::string body);
		void	setCurrentLine(std::string currentLine);
		void	setIndex(int index);
		void	setLen(int len);
		void	setFlag(bool flag);

		//Functions
		void	trimming(std::string str);

	private:
		//RAW DATA
		//to handle the raw dta received from the socket-->should be dynamic!
		std::string	_Buffer;

		//REQUEST LINE COMPONENTS
		//HTTP method
		std::string	_HttpMethod;
		//URL
		std::string	_Url;
		//HTTP version
		std::string _HttpVers;
		
		//HEADERS
		//Header's storage
		std::map <std::string, std::string> _HeadStorage;
		//Indiv Header names and values
		//header name host
		std::string	_Host;
		//header name User agent
		std::string	_UserAgent;
		//header name accept
		std::string _Accept;

		//BODY OF THE REQUEST
		//request-->to be a string or dynamic array
		std::string	_Body;
		
		//AUXILIARY VARIABLES FOR PARSING-->can be created within the func itself
		//current line being parsed
		std::string	_CurrentLine;
		//position or index vars
		int _Index;
		//length or size of variables
		int _Len;

		//FLAGS AND STATUS INDICATORS-->can be replaced by Enum
		//parsing status
		bool	_Flag;
};

#endif
