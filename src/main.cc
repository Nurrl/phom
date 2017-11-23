#include <iostream>

#include <yaml-cpp/yaml.h>

#include "log.h"
#include "sockets/server.h"

int main(int argc, char *argv[]) {
  /* Set log level to Debug */
  Log::getInstance().setLevel(Log::LOG_DEBUG);

  YAML::Node config = YAML::LoadFile("../extra/vapors.yml");
  Log::getInstance().logInfo(config["secret"].as<std::string>());

  /*Server("0.0.0.0", 1234).start([](Server* sInst, Session* cInst) {
    sInst->stop();
    cInst->closeConn();
  });*/
  return 0;
}
