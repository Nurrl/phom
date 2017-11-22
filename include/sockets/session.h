#ifndef SESSION_H
#define SESSION_H

#include <regex>
#include <string>
#include <unistd.h>

// Sockets dependencies
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "log.h"

#define RX_BUFFER 256
#define AUTH_KEY ""

class Session {
 public:
  Session(int);
  virtual ~Session();

  /**
   * Function used to close connection between the client
   * and the server.
   * @method closeConn
   */
  void closeConn();

  /**
   * Return the state of the client.
   * @method isActive
   * @return true or false.
   */
  bool isActive();

  /**
   * Return the client's socket file descriptor.
   * @method getFd
   * @return The file descriptor.
   */
  int getFd();

  /**
   * Receive client text request.
   * @method recvText
   * @return Client request.
   */
  std::string recvText();

  /**
   * Send text to client.
   * @method sendText
   * @param  std::string Text to send.
   */
  void sendText(std::string);

 private:
  bool clActive;
  int clSock;
};

#endif /* end of include guard: SESSION_H */
