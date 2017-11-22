#include <iostream>

#include "log.h"
#include "sockets/server.h"

int main(int argc, char *argv[]) {
  /* Set log level to Debug */
  Log::getInstance().setLevel(Log::LOG_DEBUG);

  /*Server("0.0.0.0", 1234).start([](Server* sInst, Session* cInst) {
    sInst->stop();
    cInst->closeConn();
  });*/
  return 0;
}
