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

class SocketManager {
private:
  std::vector<int> serverSocketsFds;
  std::vector<ServerParser> servers;
  std::vector<struct pollfd> pollFds;

public:
  SocketManager(std::vector<ServerParser> parser);
  ~SocketManager();

  // Getters
  std::vector<ServerParser> getServers() const;

  void createServerSockets();
};

#endif // SOCKET_MANAGER_HPP
