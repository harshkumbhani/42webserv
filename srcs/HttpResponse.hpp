/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:09:00 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/21 12:23:44 by otuyishi         ###   ########.fr       */
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

		void	respond(HttpRequest const &req);

		std::string	respond_Get(HttpRequest const &req);
		void	response_Post(HttpRequest const &req);
	
	private:

};

#endif

