/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/06/01 21:01:55 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//construction
Request::Request(){}
//destruction
Request::~Request(){}

//Requests
//-->GET<--
//purpose: retrieves data, parameters in URL, no body
//mandatory method('GET'), \
			path('/search?q=example'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->POST<--
//purpose: sends data to server, parameters in body, creates/updates resources
//mandatory method('POST'), \
			path('/submit-form'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type), \
			end of headers('\r\n\r\n') \
			body('name=John+Doe&age=30&city=NY')

//-->DELETE<--
//requests resource deletion, no body
//mandatory method('DELETE'), \
			path('/delete-profile/123'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

// POST /submit-form HTTP/1.1\r\n
// Host: www.example.com\r\n
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36\r\n
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
// Accept-Language: en-US,en;q=0.5\r\n
// Accept-Encoding: gzip, deflate, br\r\n
// Content-Type: application/x-www-form-urlencoded\r\n
// Content-Length: 27\r\n
// Connection: keep-alive\r\n
// Upgrade-Insecure-Requests: 1\r\n
// \r\n
// key1=value1&key2=value2
