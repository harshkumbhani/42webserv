/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:09:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/27 08:34:45 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "HttpRequest.hpp"
#include "EventLogger.hpp"

enum status {
  OK = 200,
  NOT_FOUND = 404,
  BAD_REQUEST = 400
};

class HttpRequest;

class HttpResponse {
	public:
		HttpResponse();
		~HttpResponse();

		std::string	_StatusLine;
		std::string	_Header;
		std::string	_Body;
		std::string	_Response;
		
		std::string	statusCode(int code);
		void		respond(HttpRequest const &req);

		std::string	respond_Get(HttpRequest const &req);
		std::string	response_Post(HttpRequest const &req);
		std::string	response_Delete(HttpRequest const &req);

		//supporting funcs
		bool	is_valid_str(const std::string &str);
		bool 	is_valid_char(char c);
	private:

};

#endif

