/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:04 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/24 10:51:06 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		std::map <std::string, std::string> _ReqLine;
		std::map <std::string, std::string> _Header;
		std::string _Body;

		void	request_blocks(std::string request);

		void	receive_request_line(std::string request);
		void	receive_request_header(std::string header);
		void	receive_request_body(std::string request);

	private:

};

#endif
