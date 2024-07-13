#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "Parser.hpp"
#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct clientState {
  bool flagHeaderRead;
  bool flagBodyRead;
  size_t bytesRead;
  size_t contentLength;
  size_t headerEndPosition;
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

  void pollin(int pollFd);
  void pollout(int pollFd);
  void acceptConnection(int pollFd);
  void checkAndCloseStaleConnections();
};

std::ostream &operator<<(std::ostream &output, const clientState &clientState);

#endif // SOCKET_MANAGER_HPP
