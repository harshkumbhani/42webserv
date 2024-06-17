#include "SocketManager.hpp"

SocketManager::SocketManager(std::vector<ServerParser> parser)
    : servers(parser) {}

SocketManager::~SocketManager() {
  std::vector<int>::iterator it;

  for (it = serverSocketsFds.begin(); it != serverSocketsFds.end(); it++) {
    close(*it);
  }
}

void SocketManager::createServerSockets() {
  std::vector<ServerParser>::iterator it;

  for (it = servers.begin(); it != servers.end(); it++) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
      throw std::runtime_error("Failed to create server socket: " +
                               std::string(strerror(errno)));
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(it->listen);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) == -1) {
      std::runtime_error("Failed to bind socket to address: " +
                         std::string(strerror(errno)));
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
      std::runtime_error("Failed to listen on socket: " +
                         std::string(strerror(errno)));
    }
    serverSocketsFds.push_back(serverSocket);
  }
}
