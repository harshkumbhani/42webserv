#include "SocketManager.hpp"
#include "HttpRequest.hpp"

volatile sig_atomic_t gServerSignal = 1;

// Constructor

SocketManager::SocketManager(std::vector<ServerParser> parser)
    : servers(parser) {
  createServerSockets();
  pollingAndConnections();
}

// Destructor

SocketManager::~SocketManager() {
  std::vector<struct pollfd>::iterator itp;
  for (itp = pollFds.begin(); itp != pollFds.end(); itp++) {
    INFO("Closing all open socket fds: " << itp->fd);
    close(itp->fd);
  }
}

// Getters

std::vector<ServerParser> SocketManager::getServers() const { return servers; }

// Signal function

void stopServerLoop(int) {
  gServerSignal = 0;
  INFO("CTRL + C signal recieved, stopping Server");
}

// Create Sockets Fds and Poll fds

void SocketManager::createServerSockets() {
  INFO("Booting servers ... ");
  std::vector<int> ports;
  std::vector<ServerParser>::iterator it;

  for (it = servers.begin(); it != servers.end(); it++) {
    it->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (it->sockfd == -1)
      throw std::runtime_error("Failed to create server socket: " +
                               std::string(strerror(errno)));
    const int enable = 1;
    if (setsockopt(it->sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
        0)
      throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");

    if (fcntl(it->sockfd, F_SETFL, O_NONBLOCK) < 0)
      throw std::runtime_error("Failed to make server socket non blocking: " +
                               std::string(strerror(errno)));

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(it->listen);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (std::find(ports.begin(), ports.end(), it->listen) == ports.end()) {
      if (bind(it->sockfd, (struct sockaddr *)&serverAddress,
               sizeof(serverAddress)) < 0)
        throw std::runtime_error("Failed to bind socket to address: " +
                                 std::string(strerror(errno)));

      if (listen(it->sockfd, SOMAXCONN) < 0)
        throw std::runtime_error("Failed to listen on socket: " +
                                 std::string(strerror(errno)));

      struct pollfd pollfd;

      pollfd.fd = it->sockfd;
      pollfd.events = POLLIN;
      pollfd.revents = 0;

      pollFds.push_back(pollfd);
      serverSocketsFds.push_back(it->sockfd);
      ports.push_back(it->listen);
    }
  }
}

// Polling and Connections

void SocketManager::acceptConnection(int &pollFd) {
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen = sizeof(clientAddress);
  int clientSocket =
      accept(pollFd, (struct sockaddr *)&clientAddress, &clientAddressLen);
  if (clientSocket < 0)
    throw std::runtime_error("Failed to accept client connection: " +
                             std::string(strerror(errno)));
  if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
    throw std::runtime_error("Failed to make client socket non blocking: " +
                             std::string(strerror(errno)));
  struct pollfd clientPollFd = {clientSocket, POLLIN, 0};
  pollFds.push_back(clientPollFd);
  clientSocketsFds.push_back(clientSocket);
  clientState client = (struct clientState){};
  clients[clientSocket] = client;

  clients[clientSocket].clear();
  std::time(&clients[clientSocket].lastEventTime);
  SUCCESS("Accepted new client connection: " << clientSocket);
}

bool SocketManager::isServerFd(int pollFd) {
  if (std::find(serverSocketsFds.begin(), serverSocketsFds.end(), pollFd) !=
      serverSocketsFds.end())
    return true;
  return false;
}

bool SocketManager::isClientFd(int pollFd) {
  if (std::find(clientSocketsFds.begin(), clientSocketsFds.end(), pollFd) !=
      clientSocketsFds.end())
    return true;
  return false;
}

void SocketManager::assignServerBlock(int &pollFd) {
	auto hostIt = clients[pollFd].header.find("Host");

	if (hostIt == clients[pollFd].header.end())
		return;

	std::regex pattern(R"(([^:]+):(\d+))");
	std::smatch matches;
	std::string domain;
	int port = 0;

	if (std::regex_match(clients[pollFd].header["Host"], matches, pattern)) {
		domain = matches[1].str();
		port = std::stoi(matches[2].str());

		for (auto &server : servers) {
			if (server.server_name == domain && server.listen == port) {
				clients[pollFd].serverData = server;
				break;
			}
		}
	}
}

void SocketManager::pollin(pollfd &pollFd) {
  if (isServerFd(pollFd.fd) == true) {
    acceptConnection(pollFd.fd);
  } else {
    char buffer[4096 * 4];
    std::memset(&buffer[0], 0, sizeof(buffer));
    ssize_t bytesRead = recv(pollFd.fd, buffer, sizeof(buffer), 0);
    if (bytesRead == 0) {
      return;
    } else if (bytesRead == -1) {
      WARNING("No data available to read on socket: " << pollFd.fd);
      clients[pollFd.fd].closeConnection = true;
      return;
    }

    clients[pollFd.fd].bytesRead = bytesRead;
    clients[pollFd.fd].readString = std::string(buffer, bytesRead);

    HttpRequest::requestBlock(clients[pollFd.fd]);
    assignServerBlock(pollFd.fd);
    std::time(&clients[pollFd.fd].lastEventTime);

    HttpResponse response;
    switch (clients[pollFd.fd].method) {
    case DEFAULT:
      break;
    case POST:
      if (clients[pollFd.fd].flagHeaderRead == true &&
          clients[pollFd.fd].flagBodyRead == true) {
        clients[pollFd.fd].writeString = response.respond(clients[pollFd.fd]);
        pollFd.events = POLLOUT;
      }
      break;
    default:
      if (clients[pollFd.fd].flagHeaderRead == true) {
        clients[pollFd.fd].writeString = response.respond(clients[pollFd.fd]);
        pollFd.events = POLLOUT;
      }
      break;
    }
  }
}


void SocketManager::pollout(pollfd &pollFd) {

	HttpResponse response;

	if (clients[pollFd.fd].isForked == true) {
		clients[pollFd.fd].writeString = response.respond(clients[pollFd.fd]);
		return ;
	}

  if (clients[pollFd.fd].writeString.empty() == true) {
    WARNING("Response buffer Empty on socket: " << pollFd.fd);
    clients[pollFd.fd].clear();
    pollFd.events = POLLIN;
    return;
  }

  if (clients[pollFd.fd].bytesRead > 0) {
    ssize_t bytesSend = send(pollFd.fd, clients[pollFd.fd].writeString.c_str(),
                             clients[pollFd.fd].writeString.size(), 0);

    if (bytesSend > 0) {
      std::time(&clients[pollFd.fd].lastEventTime);
      clients[pollFd.fd].writeString.erase(0, bytesSend);
      if (clients[pollFd.fd].writeString.empty() == true) {
        SUCCESS("Response sent successfully on socket: " << pollFd.fd);
        if (clients[pollFd.fd].isKeepAlive == false) {
          clients[pollFd.fd].closeConnection = true;
        }
        pollFd.events = POLLIN;
        clients[pollFd.fd].clear();
      }
    } else if (bytesSend == 0) {
      WARNING("Empty response sent on socket: " << pollFd.fd);
    } else {
      ERROR("Failed to send a response on socket: " << pollFd.fd);
      clients[pollFd.fd].closeConnection = true;
    }
  }
}

void SocketManager::closeClientConnection(int &pollFd) {
  INFO("Closing client connection on fd: " << pollFd);

  if (close(pollFd) == -1)
    throw std::runtime_error("Failed to close client connection!");

  std::vector<int>::iterator itServerFd =
      std::find(serverSocketsFds.begin(), serverSocketsFds.end(), pollFd);
  if (itServerFd != serverSocketsFds.end())
    serverSocketsFds.erase(itServerFd);

  std::vector<int>::iterator itClientFd =
      std::find(clientSocketsFds.begin(), clientSocketsFds.end(), pollFd);
  if (itClientFd != clientSocketsFds.end())
    clientSocketsFds.erase(itClientFd);

  std::vector<struct pollfd>::iterator it;
  for (it = pollFds.begin(); it != pollFds.end(); it++) {
    if (it->fd == pollFd) {
      pollFds.erase(it);
      break;
    }
  }
  auto itmap = clients.find(pollFd);
  if (itmap != clients.end()) {
    clients.erase(pollFd);
  }
}

void SocketManager::checkAndCloseStaleConnections(struct pollfd &pollFd) {
  time_t currentTime = 0;

  std::time(&currentTime);
  if ((clients[pollFd.fd].closeConnection == true) ||
      std::difftime(currentTime, clients[pollFd.fd].lastEventTime) > 10) {

    INFO("Closing client connection on fd: " << pollFd.fd);

    if (close(pollFd.fd) == -1)
      throw std::runtime_error("Failed to close client connection!");

    std::vector<int>::iterator itServerFd =
        std::find(serverSocketsFds.begin(), serverSocketsFds.end(), pollFd.fd);
    if (itServerFd != serverSocketsFds.end())
      serverSocketsFds.erase(itServerFd);

    std::vector<int>::iterator itClientFd =
        std::find(clientSocketsFds.begin(), clientSocketsFds.end(), pollFd.fd);
    if (itClientFd != clientSocketsFds.end())
      clientSocketsFds.erase(itClientFd);

    std::vector<struct pollfd>::iterator it;
    for (it = pollFds.begin(); it != pollFds.end(); it++) {
      if (it->fd == pollFd.fd) {
        pollFds.erase(it);
        break;
      }
    }
    auto itmap = clients.find(pollFd.fd);
    if (itmap != clients.end()) {
      clients.erase(pollFd.fd);
    }
  }
}

void SocketManager::pollingAndConnections() {
  if (servers.size() < 1)
    return;
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, stopServerLoop);
  while (gServerSignal) {
    if (poll(&pollFds[0], pollFds.size(), 0) == -1 && gServerSignal == 1)
      throw std::runtime_error("Error from poll function: " +
                               std::string(strerror(errno)));

    for (size_t i = 0; i < pollFds.size(); i++) {
      if (pollFds[i].revents & POLLIN) {
        pollin(pollFds[i]);
      }
      if (pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
        checkAndCloseStaleConnections(pollFds[i]);
        continue;
      }
      if (isClientFd(pollFds[i].fd) == true) {
        if (pollFds[i].revents & POLLOUT) {
          pollout(pollFds[i]);
        }
        checkAndCloseStaleConnections(pollFds[i]);
      }
    }
  }
}

// Operator overloads

std::ostream &operator<<(std::ostream &output, const clientState &clientState) {
  output << "\nHeader Read Flag: " << clientState.flagBodyRead
         << "\nBody Read Flag: " << clientState.flagBodyRead
         << "\nbytes read: " << clientState.bytesRead
         << "\ncontent Length: " << clientState.contentLength
         << "\nstart time: " << clientState.lastEventTime
         << "\nbody: " << clientState.bodyString << std::endl;

  return output;
}
