#include "SocketManager.hpp"

SocketManager::SocketManager(std::vector<ServerParser> parser)
    : servers(parser) {
  createServerSockets();
}

SocketManager::~SocketManager() {
  std::vector<int>::iterator it;

  for (it = serverSocketsFds.begin(); it != serverSocketsFds.end(); it++) {
    close(*it);
  }
}

std::vector<ServerParser> SocketManager::getServers() const {
  return this->servers;
}

void SocketManager::createServerSockets() {
  INFO("Booting servers ... ");
  std::vector<ServerParser>::iterator it;

  for (it = servers.begin(); it != servers.end(); it++) {
    it->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    DEBUG("sockfd: " << it->sockfd);
    if (it->sockfd == -1) {
      throw std::runtime_error("Failed to create server socket: " +
                               std::string(strerror(errno)));
    }

    const int enable = 1;
    if (setsockopt(it->sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
        0) {
      throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
    }

    if (fcntl(it->sockfd, F_SETFL, O_NONBLOCK) < 0) {
      throw std::runtime_error("Failed to make server socket non blocking: " +
                               std::string(strerror(errno)));
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(it->listen);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(it->sockfd, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0) {
      std::runtime_error("Failed to bind socket to address: " +
                         std::string(strerror(errno)));
    }

    if (listen(it->sockfd, SOMAXCONN) < 0) {
      std::runtime_error("Failed to listen on socket: " +
                         std::string(strerror(errno)));
    }

    struct pollfd pollfd;

    pollfd.fd = it->sockfd;
    pollfd.events = POLLIN;
    pollfd.revents = 0;

    pollFds.push_back(pollfd);
    DEBUG("server: " + it->server_name + ":" << it->listen);
  }
}
