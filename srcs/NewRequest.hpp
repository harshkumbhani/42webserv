/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:24:54 by otuyishi          #+#    #+#             */
/*   Updated: 2024/08/05 09:56:09 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEWREQUEST_HPP
#define NEWREQUEST_HPP

#include "Structs.hpp"
#include "EventLogger.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class NewRequest {
	public:
		NewRequest();
		~NewRequest();

		// std::map<std::string, std::string> reqLine;
		// std::map<std::string, std::string> header;
		// std::string body;

		static std::vector<std::string> chunkRequest(clientState &state);
		static std::map<std::string, std::string> parameterizeHeader(const std::string& headerStr, clientState &state);
		static std::string trim(const std::string& str);
		static std::map<std::string, std::string>	parameterizeRequestLine(std::string	rLine, clientState &state);
		static void	requestBlock(clientState &state);
		static std::string::size_type findDelimiter(clientState &state, \
			const std::string& delimiter, std::string::size_type start);

};

#endif
