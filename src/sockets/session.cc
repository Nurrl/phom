#include "sockets/session.h"

Session::Session(int givenSocket) {
  this->clSock = givenSocket;
  this->clActive = true;

  Log::getInstance().logDebug("* Session id '" + std::to_string(this->clSock) + "' opened");
}

Session::~Session() {
  shutdown(this->clSock, SHUT_RDWR);
  close(this->clSock);

  Log::getInstance().logDebug("* Session id '" + std::to_string(this->clSock) + "' closed");
}

void Session::closeConn() {this->clActive = false;}

bool Session::isActive() {return this->clActive;}

int Session::getFd() {return this->clSock;}

std::string Session::recvText() {
  /* Define buffer */
  char rxBuf[RX_BUFFER] = "";

  /* Read message from client if any */
  struct pollfd fd = {
    .fd = this->clSock,
    .events = POLLIN
  };
  int pollState = poll(&fd, 1, 200);
  if (pollState < 0)
    perror("Error -> On poll()"), _exit(1);
  else if (pollState > 0) {
    ssize_t readState = recv(this->clSock, rxBuf, RX_BUFFER - 1, MSG_DONTWAIT);
    if (readState < 0)
      perror("Error -> Can't read from socket"), _exit(1);
  }

  /* Return it */
  return std::regex_replace(std::string(rxBuf), std::regex("^( |\r|\n|\r\n|\t)+|( |\r|\n|\r\n|\t)+$"), "");
}

void Session::sendText(std::string textMessage) {
  /* Send message to client */
  ssize_t writeState = write(this->clSock, textMessage.c_str(), textMessage.size());
  if (writeState < 0)
    perror("Error -> Can't write to socket"), _exit(1);
}
