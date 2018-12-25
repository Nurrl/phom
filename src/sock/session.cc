#include "sock/session.h"

namespace Sock {

Session::Session(int givenSocket) {
  this->sSock = givenSocket;
  this->sActive = true;

  Log::getInstance().logDebug("* Session id '" + std::to_string(this->sSock) + "' opened");
}

Session::~Session() {
  shutdown(this->sSock, SHUT_RDWR);
  close(this->sSock);

  Log::getInstance().logDebug("* Session id '" + std::to_string(this->sSock) + "' closed");
}

void Session::end() { this->sActive = false; }

bool Session::isActive() { return this->sActive; }

int Session::getFd() { return this->sSock; }

void Session::sendText(std::string textMessage) {
  /* Send message to client */
  ssize_t writeState = send(this->sSock, textMessage.c_str(), textMessage.size(), 0);
  if (writeState < 0)
    throw std::runtime_error("Error -> Can't write to socket");
}

std::string Session::recvText() {
  /* Define buffers */
  char rxBuf[255] = "";
  std::string rxStr;

  /* Read message from client if any */
  struct pollfd fd = {
      .fd = this->sSock,
      .events = POLLIN
  };
  int pollState = poll(&fd, 1, 200);
  if (pollState < 0)
    throw std::runtime_error("Error -> On poll()");
  else if (pollState > 0) {
    ssize_t bytesRead;

    while ((bytesRead = recv(this->sSock, rxBuf, sizeof rxBuf, MSG_DONTWAIT)) > 0) {
      Log::getInstance().logSimple(std::to_string(bytesRead));
      rxStr += rxBuf;

      memset(rxBuf, 0, sizeof rxBuf);
    }

    if (bytesRead < 0 && errno != EAGAIN)
      throw std::runtime_error("Error -> Can't read from socket");
  }

  /* Return it */
  return rxStr;
}

void Session::setFlash(std::string flashName, std::string flashValue) {
  this->flashMap[flashName] = flashValue;
}

std::string Session::getFlash(std::string flashName) {
  return (this->flashMap.find(flashName) != this->flashMap.end())
    ? this->flashMap[flashName]
    : "";
}

}; /* End of namespace: Sock */
