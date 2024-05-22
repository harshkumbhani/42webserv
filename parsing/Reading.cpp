/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reading.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:55:01 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/22 10:53:39 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reading.hpp"

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

//-->PUT<--
//purpose: sends full data to server, parameters in body, creates/updates resources
//mandatory method('PUT'), \
			path('/update-profile'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type), \
			end of headers('\r\n\r\n')

//-->DELETE<--
//requests resource deletion, no body
//mandatory method('DELETE'), \
			path('/delete-profile/123'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->HEAD<--
//purpose: retrieves headers, no body
//mandatory method('HEAD'), \
			path('/check-resource'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->OPTIONs<--
//purpose: Describes communication options, no body
//mandatory method('OPTIONs'), \
			path('/api/resource'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent,accept)

//-->PATCH<--
//purpose: sends partial data to update a resource, parameters in body
//mandatory method('PATCH'), \
			path('/update-profile'), \
			http version('HTTP/1.1'), \
			headers(host, content-length, user_agent, content-type) \
			end of headers('\r\n\r\n') \
			Body({"email":"email@example.com"})

//-->CONNECT<--
//purpose: establishes a tunnel, no typical body in inial request
//mandatory method('CONNECT'), \
			Host and port('example.com:443'), \
			http version('HTTP/1.1'), \
			end of headers('\r\n\r\n')
//optional headers(host, user_agent, 'Proxy-Authorization')
