#include "sock/server.h"

namespace Sock {

Server::Server(std::string listenAddr, uint16_t listenPort) {
  Log::getInstance().logInfo("Initializing socket server");

  this->srvSock = socket(AF_INET, SOCK_STREAM, 0);
  if (this->srvSock < 0)
    throw std::runtime_error("Error -> Failed to open socket");

  int reuseAddr = 1;
  if (setsockopt(this->srvSock, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) < 0)
    throw std::runtime_error("Error -> Failed to reuse addr");

  Log::getInstance().logInfo("Binding to " + listenAddr + ":" + std::to_string(listenPort));

  /* Bind socket */
  struct sockaddr_in srvAddr{};
  srvAddr.sin_family = AF_INET;
  inet_pton(AF_INET, listenAddr.c_str(), &(srvAddr.sin_addr));
  srvAddr.sin_port = htons(listenPort);

  if (bind(this->srvSock, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0)
    throw std::runtime_error("Error -> Can't bind socket");

  this->srvActive = true;
}

Server::~Server() {
  /* Disconnect all active clients */
  for (auto sObj : sList)
    if (sObj->isActive())
      sObj->end();

  /* Join all threads */
  for (auto &thObj : thList)
    thObj.join();

  Log::getInstance().logDebug("Closing socket");
  shutdown(this->srvSock, SHUT_RDWR);
  close(this->srvSock);
}

void Server::start(std::function<void(Server *, Session *)> callMe) {
  Log::getInstance().logInfo("Starting socket server");

  /* Start listening */
  if (listen(this->srvSock, MAX_QUEUE) < 0)
    throw std::runtime_error("Error -> Can't start listening");

  /* Run socket Server */
  while (this->srvActive) {
    /* Accept client ONLY if one try to connect */
    struct pollfd fd = {
        .fd = this->srvSock,
        .events = POLLIN
    };
    int pollState = poll(&fd, 1, SEEK_MS);
    if (pollState < 0)
      throw std::runtime_error("Error -> On poll()");
    else if (pollState > 0) {
      /* Set socket to accept mode and wait for clients */
      struct sockaddr_in clientAddr{};
      socklen_t clientAddrLen = sizeof(clientAddr);

      int clSock = accept(this->srvSock, (struct sockaddr *) &clientAddr, &clientAddrLen);
      if (clSock < 0)
        throw std::runtime_error("Error -> On accept");

      /* Handle client on new thread */
      thList.emplace_back(std::thread(&Server::handleClient, this, callMe, clSock));
    }

    /* Check for disconnected sessions */
    char *c;
    for (auto sObj : sList)
      if (recv(sObj->getFd(), &c, 1, MSG_DONTWAIT | MSG_PEEK) == 0)
        sObj->end();
  }
}

void Server::stop() {
  Log::getInstance().logInfo("Stopping socket server");
  this->srvActive = false;
}

void Server::handleClient(std::function<void(Server *, Session *)> callMe, int clSock) {
  /* Init client object and push reference to sList */
  Session sObj(clSock);
  sList.emplace_back(&sObj);

  /* Call the handle with reference to socket Server and client object */
  while (sObj.isActive()) { callMe(this, &sObj); }

  /* Remove it from active client list */
  sList.erase(std::remove(sList.begin(), sList.end(), &sObj), sList.end());
}

}; /* End of namespace: Sock */