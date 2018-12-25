#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <netdb.h>

#include "sock/session.h"

namespace Sock {

/*! @brief This class is used to make a client connection to a server listening to a TCP port. */
class Client {
public:
  /*! @brief This creates a network connection to a server using his IPv4 Address and port.
   *
   * This creates a socket connection using an IPv4 address and a TCP port to a listening server,
   * and creates a Session object with the connected file descriptor to handle the connection easily.
   * However, the file descriptor can be obtained by calling @a Session::getFd() for further use.
   *
   * @param [in] serverAddr The server's IPv4 address, E.g. 'a.b.c.d'.
   * @param [in] serverPort The server's TCP port to connect to.
   *
   * @throw ...
   */
  Client(std::string serverAddr, uint16_t serverPort);

  /*! @brief This deletes the @a clSess pointer to free memory. */
  virtual ~Client();

  /*! @brief Returns a pointer to the Session opened by the constructor.
   * @return A pointer to the opened Session object.
   */
  Session *getSession();

private:
  Session *clSess; /*!< The pointer to the Session opened by the constructor. */
}; /* End of class: Client */

}; /* End of namespace: Sock */

#endif /* end of include guard: CLIENT_H */
