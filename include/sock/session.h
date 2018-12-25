#ifndef SESSION_H
#define SESSION_H

#include <map>
#include <regex>
#include <string>

// Sockets dependencies
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "log.h"

namespace Sock {

/*! @brief */
class SockExcept : public std::exception {
public:
  /*! @brief */
  SockExcept() : expStr("Unknown error with socket.") {}

  /*! @brief
   *
   * @param expStr
   */
  SockExcept(std::string expStr) : expStr(expStr) {}

  /*! @brief
   *
   * @return
   */
  virtual const char *what() const throw() {return expStr.c_str();}

private:
  std::string expStr;
}; /* End of class: SockExcept */

/*! @brief */
class Session {
 public:
  /*! @brief Constructs the Session with a socket provided by the Client or the Server class.
   *
   * @param [in] givenSocket
   */
  Session(int givenSocket);

  /*! @brief Basic destructor */
  virtual ~Session();

  /*! @brief
   *
   */
  void end();

  /*! @brief
   *
   * @return
   */
  bool isActive();

  /*! @brief
   *
   * @return
   */
  int getFd();

  /*! @brief
   *
   * @param [in] textMessage
   */
  void sendText(std::string textMessage);

  /*! @brief
   *
   * @return
   */
  std::string recvText();

  /*! @brief
   *
   * @param [in] flashName
   * @param [in] flashValue
   */
  void setFlash(std::string flashName, std::string flashValue);

  /*! @brief
   *
   * @param [in] flashName
   * @return
   */
  std::string getFlash(std::string flashName);

 private:
  bool sActive; /*!< Keeps track of the Session active status. */
  int sSock; /*!< Session's socket file descriptor. */

  std::map<std::string, std::string> flashMap; /*!< Stores volatile data for the current session. */
}; /* End of class: Session */

}; /* End of namespace: Sock */

#endif /* end of include guard: SESSION_H */
