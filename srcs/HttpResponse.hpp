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
#define HTTPRESPONSE_HPP

#include "EventLogger.hpp"
#include "HttpRequest.hpp"
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

enum status { OK = 200, NOT_FOUND = 404, BAD_REQUEST = 400 };

class HttpRequest;

class HttpResponse {
public:
  HttpResponse();
  ~HttpResponse();

  std::string _StatusLine;
  std::string _Header;
  std::string _Body;
  std::string _Response;

  std::string statusCode(int code);
  std::string respond(clientState &req);

  std::string respond_Get(clientState &req);
  std::string response_Post(clientState &req);
  std::string response_Delete(clientState &req);

  // supporting funcs
  bool is_valid_str(const std::string &str);
  bool is_valid_char(char c);

private:
};

#endif
