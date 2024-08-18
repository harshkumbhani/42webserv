/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/15 19:35:04 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "Structs.hpp"
#include "EventLogger.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "HttpResponse.hpp"

class HttpResponse;

class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		std::map<std::string, std::string> reqLine;
		std::map<std::string, std::string> header;
		std::string body;

		static void	requestBlock(clientState &clientData);
		static void	parseRequestLine(clientState &clientData, std::string &request);
		static void	parseRequestHeader(clientState &clientData, std::string &reqheader);
};

#endif
