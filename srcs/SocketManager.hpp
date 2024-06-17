#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "Parser.hpp"
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

class SocketManager {
private:
  std::vector<int> serverSocketsFds;
  std::vector<ServerParser> servers;

public:
  SocketManager(std::vector<ServerParser> parser);
  ~SocketManager();

  void createServerSockets();
};

#endif // SOCKET_MANAGER_HPP
