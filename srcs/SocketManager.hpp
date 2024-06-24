#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "Parser.hpp"
#include <algorithm>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct clientState {
  size_t bytesRead;
  size_t contentLength;
  time_t startTime;
  std::string body;
  std::string readString;
  std::string writeString;
  std::map<std::string, std::string> requestLine;
  std::map<std::string, std::string> header;
  ServerParser serverData;
};

class SocketManager {
private:
  std::vector<int> serverSocketsFds;
  std::vector<ServerParser> servers;
  std::vector<struct pollfd> pollFds;
  std::vector<struct clientState> clients;
  // std::vector<struct pollfd> clientSocketFds;

public:
  SocketManager(std::vector<ServerParser> parser);
  ~SocketManager();

  // Getters
  std::vector<ServerParser> getServers() const;

  void createServerSockets();
  void pollingAndConnections();

  void pollin(int pollFd);
  void pollout(int pollFd);
  void acceptConnection(int pollFd);
};

#endif // SOCKET_MANAGER_HPP
