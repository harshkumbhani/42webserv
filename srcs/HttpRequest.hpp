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
#define HTTPREQUEST_HPP

#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class HttpRequest {
public:
  HttpRequest();
  ~HttpRequest();

  std::map<std::string, std::string> ReqLine;
  std::map<std::string, std::string> Header;
  std::string Body;

  void requestBlock(std::string &request);

  void parseRequestLine(std::string &request);
  void parseRequestHeader(std::string &header);
  void parseRequestBody(std::string &request);

private:
};

#endif
