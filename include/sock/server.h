#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <string>
#include <vector>

// Sockets dependencies
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sock/session.h"
#include "log.h"

#define MAX_QUEUE 32
#define SEEK_MS 100

namespace Sock {

/*! @brief */
class Server {
 public:
  /*! @brief
   *
   * @param [in] listenAddr
   * @param [in] listenPort
   */
  Server(std::string listenAddr, uint16_t listenPort);
  
  /*! @brief */
  virtual ~Server();

  /*! @brief
   *
   * @param [in] callMe
   */
  void start(std::function<void(Server*, Session*)> callMe);

  /*! @brief
   *
   */
  void stop();

 private:

  /*! @brief
   *
   * @param [in] callMe
   * @param [in] clSock
   */
  void handleClient(std::function<void(Server*, Session*)> callMe, int clSock);

  std::vector<std::thread> thList; /*!< Running thread array, for clean shutdown. */
  std::vector<Session*> sList; /*!< Opened Session array, for clean shutdown. */
  bool srvActive; /*!< Keeps track of the Server active status. */
  int srvSock; /*!< Server's socket file descriptor. */
}; /* End of class: Server */

}; /* End of namespace: Sock */

#endif /* end of include guard: SERVER_H */
