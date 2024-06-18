#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "Parser.hpp"
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <poll.h>
#include <algorithm>

class SocketManager {
private:
  std::vector<int> serverSocketsFds;
  std::vector<ServerParser> servers;
  std::vector<struct pollfd> pollFds;
  // std::vector<struct pollfd> clientSocketFds;

public:
  SocketManager(std::vector<ServerParser> parser);
  ~SocketManager();

  // Getters
  std::vector<ServerParser> getServers() const;

  void createServerSockets();
  void pollingAndConnections();

  void pollin(int pollFd);
  void acceptConnection(int pollFd);
};

#endif // SOCKET_MANAGER_HPP
