#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "HttpResponse.hpp"
#include "Parser.hpp"
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class SocketManager {
private:
  std::vector<int> serverSocketsFds;
  std::vector<int> clientSocketsFds;
  std::vector<ServerParser> servers;
  std::vector<struct pollfd> pollFds;
  std::map<int, clientState> clients;

public:
  // typedefs
  typedef std::vector<struct pollfd>::iterator pollFdsIterator;

  SocketManager(std::vector<ServerParser> parser);
  ~SocketManager();

  // Getters
  std::vector<ServerParser> getServers() const;

  // other methods for socket manipulation
  void createServerSockets();
  void pollingAndConnections();

  // Bool return methods for checking if its client or server fd
  bool isServerFd(int pollFd);
  bool isClientFd(int pollFd);

  void pollin(pollfd &pollFd);
  void pollout(pollfd &pollFd);
  void acceptConnection(int &pollFd);
  void checkAndCloseStaleConnections(struct pollfd &pollfd);
  void closeClientConnection(int &pollFd);
};

std::ostream &operator<<(std::ostream &output, const clientState &clientState);

#endif // SOCKET_MANAGER_HPP
