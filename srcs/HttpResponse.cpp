/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:08:24 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/19 12:11:22 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

void	HttpResponse::handle_request(HttpRequest const &req) {
	std::string	url;
	std::map<std::string, std::string>::const_iterator	it = req.ReqLine.find("url");
	if (it != req.ReqLine.end()) {
		url = it->second;
	}else{
		url = "";
	}
}
