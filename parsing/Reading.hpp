/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reading.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/29 19:38:27 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READING_HPP
# define READING_HPP

#include <iostream>
#include <map>

class Reading {
	public:
		//construction
		Reading(){}
		//destruction
		~Reading(){}

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
