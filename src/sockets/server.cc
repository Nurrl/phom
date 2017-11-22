#include "sockets/server.h"

Server::Server(std::string listenAddr, uint16_t listenPort) {
  Log::getInstance().logInfo("Initializing socket server");

  srvSock = socket(AF_INET, SOCK_STREAM, 0);
  if (srvSock < 0)
    perror("Error -> Failed to open socket"), _exit(1);

  int reuseAddr = 1;
  if (setsockopt(srvSock, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) < 0)
    perror("Error -> Failed to enable reusing address");

  Log::getInstance().logInfo("Binding to " + listenAddr + ":" + std::to_string(listenPort));

  /* Bind socket */
  struct sockaddr_in srvAddr{};
  srvAddr.sin_family = AF_INET;
  inet_pton(AF_INET, listenAddr.c_str(), &(srvAddr.sin_addr));
  srvAddr.sin_port = htons(listenPort);

  if (bind(this->srvSock, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0)
    perror("Error -> Can't bind socket"), _exit(1);

  this->srvActive = true;
}

Server::~Server() {
  /* Disconnect all active clients */
  for (auto clObj : clList)
    if (clObj->isActive())
      clObj->closeConn();

  /* Join all threads */
  for (auto &thObj : thList)
    thObj.join();

  Log::getInstance().logDebug("Closing socket");
  shutdown(this->srvSock, SHUT_RDWR);
  close(this->srvSock);
}

void Server::start(std::function<void(Server*, Session*)> handleBack) {
  Log::getInstance().logInfo("Starting socket server");

  /* Start listening */
  if (listen(this->srvSock, MAX_QUEUE) < 0)
    perror("Error -> Can't start listening"), _exit(1);

  /* Run socket server */
  while(this->srvActive) {
    /* Accept client ONLY if one try to connect */
    struct pollfd fd = {
      .fd = this->srvSock,
      .events = POLLIN
    };
    int pollState = poll(&fd, 1, SEEK_MS);
    if (pollState < 0)
      perror("Error -> On poll()"), _exit(1);
    else if (pollState > 0) {
      /* Set socket to accept mode and wait for clients */
      struct sockaddr_in clientAddr{};
      socklen_t clientAddrLen = sizeof(clientAddr);

      int clSock = accept(this->srvSock, (struct sockaddr *) &clientAddr, &clientAddrLen);
      if (clSock < 0)
        perror("Error -> On accept"), _exit(1);

      /* Handle client on new thread */
      thList.emplace_back(std::thread(&Server::handleClient, this, handleBack, clSock));
    }

    /* Check for disconnected clients */
    char* c;
    for (auto clObj : clList)
      if(recv(clObj->getFd(), &c, 1, MSG_DONTWAIT | MSG_PEEK) == 0)
        clObj->closeConn();
  }
}

void Server::stop() {
  Log::getInstance().logInfo("Stopping socket server");
  this->srvActive = false;
}

void Server::handleClient(std::function<void(Server*, Session*)> handleBack, int clSock) {
  /* Init client object and push reference to clList */
  Session clObj(clSock);
  clList.emplace_back(&clObj);

  /* Call the handle with reference to socket server and client object */
  while(clObj.isActive()) {handleBack(this, &clObj);}

  /* Remove it from active client list */
  clList.erase(std::remove(clList.begin(), clList.end(), &clObj), clList.end());
}
