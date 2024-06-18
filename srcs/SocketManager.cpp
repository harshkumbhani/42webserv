#include "SocketManager.hpp"

// Constructor
SocketManager::SocketManager(std::vector<ServerParser> parser)
    : servers(parser) {
  createServerSockets();
  pollingAndConnections();
}

// Desctructor
SocketManager::~SocketManager() {
  std::vector<int>::iterator it;

  for (it = serverSocketsFds.begin(); it != serverSocketsFds.end(); it++) {
    close(*it);
  }
}

// Getters

std::vector<ServerParser> SocketManager::getServers() const {
  return this->servers;
}

// Create Sockets Fds and Poll fds

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
    serverSocketsFds.push_back(it->sockfd);
    DEBUG("server: " + it->server_name + ":" << it->listen);
  }
}

// Polling and Connections

void SocketManager::pollin(int pollFd) {
  if (std::find(serverSocketsFds.begin(), serverSocketsFds.end(), pollFd) !=
      serverSocketsFds.end()) {
    acceptConnection(pollFd);
  } else {
    char buffer[100000];
	DEBUG("READING FROM " << pollFd);
    size_t bytesread = recv(pollFd, buffer, sizeof(buffer), 0);
	std::cout << "B: " << bytesread << std::endl;
    std::cout << std::string(buffer) << std::endl;
    exit(42);
  }
}

void SocketManager::acceptConnection(int pollFd) {
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen = sizeof(clientAddress);
  int clientSocket =
      accept(pollFd, (struct sockaddr *)&clientAddress, &clientAddressLen);
  if (clientSocket < 0) {
    throw std::runtime_error("Failed to accept client connection: " +
                             std::string(strerror(errno)));
  }
  if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
    throw std::runtime_error("Failed to make client socket non blocking: " +
                             std::string(strerror(errno)));
  }
  struct pollfd clientPollFd = {clientSocket, POLLIN, 0};
  pollFds.push_back(clientPollFd);
  SUCCESS("Accepted new client connection: " << clientSocket);
}

void SocketManager::pollingAndConnections() {
  if (servers.size() < 1)
    return;
  while (true) {
    int pollEvent = poll(&pollFds[0], pollFds.size(), servers[0].send_timeout);
    if (pollEvent == 0)
      continue;
    if (pollEvent < 0)
      throw std::runtime_error("Error from poll function: " +
                               std::string(strerror(errno)));

	DEBUG("Looping");
    std::vector<struct pollfd>::iterator it;
	for (size_t i = 0; i < pollFds.size(); i++) {
		if (pollFds[i].revents & POLLIN) {
			pollin(pollFds[i].fd);
		}
	}
    //for (it = pollFds.begin(); it != pollFds.end(); ++it) {
	//DEBUG("Checking SOcket: " << it->fd);
    //  if (it->revents & POLLIN) {
    //    pollin(it->fd);
    //  }
    //}
  }
}
