#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "Structs.hpp"
#include "EventLogger.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "HttpResponse.hpp"
#include "SocketManager.hpp"

class HttpResponse;

class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		static void	requestBlock(clientState &clientData, std::vector<ServerParser> &servers);
		static void	parseRequestLine(clientState &clientData, std::string &request);
		static void	parseRequestHeader(clientState &clientData, std::string &reqheader);
};

#endif