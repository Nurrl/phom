#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <algorithm>
#include "log.h"

class ObjectType {
 public:
  virtual ~ObjectType() {}

  virtual std::string toString() = 0;
  virtual bool isTrue() const = 0;
};
class IntType: public ObjectType {
 public:
  IntType(int value): value(value) {}

  virtual std::string toString() {return std::to_string(value);}
  bool isTrue() const {return value != 0;}

 private:
  int value;
};
class FloatType: public ObjectType {
 public:
  FloatType(float value): value(value) {}

  virtual std::string toString() {return std::to_string(value);}
  bool isTrue() const {return value != 0.0;}

 private:
  float value;
};
class StringType: public ObjectType {
 public:
  StringType(std::string value): value(value) {}

  virtual std::string toString() {return value;}
  bool isTrue() const {return !value.empty();}

 private:
  std::string value;
};

class ControlNode {
 public:
  std::string nodeVal;
  size_t nodePos, nodeEnd;

  std::vector<std::string> toVector() {
    std::istringstream iss(nodeVal);
    std::vector<std::string> tokens;
    for (std::string s; std::getline(iss, s, ' '); tokens.push_back(s));

    return tokens;
  }
};

class RenderException: public std::exception {
 public:
  RenderException(): expStr("Unknown error while parsing a file.") {}
  RenderException(std::string expStr): expStr(expStr) {}

  virtual char const *what() const throw(){return expStr.c_str();}
 private:
  std::string expStr;
};

class Template {
 public:
  Template(std::string);
  virtual ~Template();

  void set(std::string, int);
  void set(std::string, float);
  void set(std::string, std::string);

  std::string render();

 private:
  const std::string sourceCode;
  std::map<std::string, ObjectType*> valueByName;
};

#endif /* end of include guard: TEMPLATE_H */
