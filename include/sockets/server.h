#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <string>
#include <vector>
#include <unistd.h>

// Sockets dependencies
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "sockets/session.h"
#include "log.h"

#define MAX_QUEUE 32
#define SEEK_MS 100

class Server {
 public:
  /**
   * Create instance of the server and bind to port and address
   * @param  std::string Listen address.
   * @param  uint16_t    Listen port.
   */
  Server(std::string, uint16_t);
  virtual ~Server();

  /**
   * Start to listen on bound address
   * @param std::function<void(Server*, Session*)> The callback method.
   */
  void start(std::function<void(Server*, Session*)>);

  /**
   * Stop the server by breaking the main loop.
   * @method stop
   */
  void stop();

 private:
  /**
   * Method used to handle client once he's connected
   * @method handleClient
   * @param  std::function<void(Server*, Session*)> Callback method to be called.
   * @param  int Client socket file descriptor.
   */
  void handleClient(std::function<void(Server*, Session*)>, int clientSock);

  std::vector<Session*> clList;
  std::vector<std::thread> thList;
  bool srvActive;
  int srvSock;
};

#endif /* end of include guard: SERVER_H */
