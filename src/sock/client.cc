#include "sock/client.h"

namespace Sock {

Client::Client(std::string serverAddr, uint16_t serverPort) {
  int clSock;

  /* Create a socket point */
  clSock = socket(AF_INET, SOCK_STREAM, 0);
  if (clSock < 0)
    throw std::runtime_error("ERROR opening socket");

  struct hostent *server = gethostbyname(serverAddr.c_str());

  if (server == nullptr)
    throw std::runtime_error("ERROR, no such host");

  struct sockaddr_in srvAddr{};
  srvAddr.sin_family = AF_INET;
  inet_pton(AF_INET, server->h_addr, &(srvAddr.sin_addr));
  srvAddr.sin_port = htons(serverPort);

  /* Now connect to the server */
  if (connect(clSock, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0) {
    throw std::runtime_error("ERROR connecting");
  }

  this->clSess = new Session(clSock);
}

Client::~Client() {
  delete this->clSess;
}

Session *Client::getSession() {
  return this->clSess;
}

}; /* End of namespace: Sock */