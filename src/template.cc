#include "template.h"

Template::Template(std::string sourceCode): sourceCode(sourceCode) {}
Template::~Template() {}

void Template::set(std::string name, std::string value) {
  valueByName[name] = new StringType(value);
}
void Template::set(std::string name, float value) {
  valueByName[name] = new FloatType(value);
}
void Template::set(std::string name, int value) {
  valueByName[name] = new IntType(value);
}

std::string Template::render() {
  /* Find nodes recursively */
  std::vector<ControlNode> nodeList;
  for(size_t nodePos = 0; (nodePos = sourceCode.find("{~", nodePos)) != std::string::npos; ++nodePos) {
    /* Find the closing marker for the node */
    size_t nodeEnd = sourceCode.find("}", nodePos) + 1;

    /* Calculate line number */
    size_t lineNum = std::count(sourceCode.begin(), sourceCode.begin() + nodePos, '\n') + 1;

    /* Throw an error if the node is not closed on the same line */
    if (sourceCode.find("}", nodePos) == std::string::npos || sourceCode.find("\n", nodePos) < nodeEnd) {
      throw RenderException("Cannot find matching closing marker for the opened node at line " + std::to_string(lineNum));
    }

    /* Get control node content */
    std::string controlNode = sourceCode.substr((nodePos + 2), (nodeEnd - 1) - (nodePos + 2));

    /* Push new node into the list */
    ControlNode node;
    node.nodePos = nodePos;
    node.nodeEnd = nodeEnd;
    node.nodeVal = controlNode;
    nodeList.push_back(node);
  }

  /* Process nodes */
  std::string localCode = "";
  for(size_t i = 0; i < nodeList.size(); ++i) {
    ControlNode node = nodeList[i];
    std::vector<std::string> nodeVector = node.toVector();

    /* Calculate line number */
    size_t lineNum = std::count(sourceCode.begin(), sourceCode.begin() + node.nodePos, '\n') + 1;

    if(nodeVector[0] == "if") {
      bool invertNode = (nodeVector[1] == "not");
      std::string nodeName = (invertNode) ? nodeVector[2] : nodeVector[1];

      /* Try to match node name to value */
      bool nodeVal;
      if(valueByName[nodeName] != nullptr) {
        nodeVal = (invertNode)
          ? !valueByName[nodeName]->isTrue()
          : valueByName[nodeName]->isTrue();
      } else {
        throw RenderException("Value for '" + nodeName + "' cannot be found at line " + std::to_string(lineNum));
      }

      /* Find closing node and define bounds of the block to be replaced */
      size_t blockPos = 0;
      std::string selectedBlock = "";

      bool isElse; // Find the type of 'if' statement
      if(nodeList[i + 1].nodeVal == "else" && nodeList[i + 2].nodeVal == "endif") { // Process as if..else..endif
        isElse = true;
      } else if(nodeList[i + 1].nodeVal == "endif") {  // Process as if..endif
        isElse = false;
      } else {
        throw RenderException("Cannot find matching closing node for the opened one at line " + std::to_string(lineNum));
      }

      if(isElse) {
        blockPos = node.nodePos;

        if(nodeVal) { // If -> Else
          selectedBlock = sourceCode.substr(node.nodeEnd,
            nodeList[i + 1].nodePos - node.nodeEnd);
        } else { // Else -> Endif
          selectedBlock = sourceCode.substr(nodeList[i + 1].nodeEnd,
            nodeList[i + 2].nodePos - nodeList[i + 1].nodeEnd);
        }
      } else {
        blockPos = node.nodePos;

        if(nodeVal) { // If -> Endif
          selectedBlock = sourceCode.substr(node.nodeEnd,
            nodeList[i + 1].nodePos - nodeList[i + 1].nodeEnd);
        }
      }

      /* Trim trailing and following whitespaces and newlines */
      if(selectedBlock != "") {
        size_t strBegin = selectedBlock.find_first_not_of("\n ");
        size_t strEnd = selectedBlock.find_last_not_of("\n ");

        selectedBlock = selectedBlock.substr(strBegin, strEnd - strBegin + 1);
      }

      /* Add code preceding to localCode */
      if(i == 0) { // If first pass
        localCode += sourceCode.substr(0, blockPos - 0);
      } else {
        localCode += sourceCode.substr(nodeList[i - 1].nodeEnd, blockPos - nodeList[i - 1].nodeEnd);
      }

      /* Add processed node to localCode */
      localCode += selectedBlock;

      /* If last control node, add end of file to localCode */
      if(isElse && i + 2 == nodeList.size() - 1) {
        localCode += sourceCode.substr(nodeList[i + 2].nodeEnd, sourceCode.size() - nodeList[i + 2].nodeEnd);
      } else if (!isElse && i + 1 == nodeList.size() - 1) {
        localCode += sourceCode.substr(nodeList[i + 1].nodeEnd, sourceCode.size() - nodeList[i + 1].nodeEnd);
      }
    } else if(nodeVector[0] == "else") {
      if(nodeList[i - 1].toVector()[0] == "if") {
        continue;
      } else {
        throw RenderException("Unexcpected 'else' node at line " + std::to_string(lineNum));
      }
    } else if(nodeVector[0] == "endif") {
      if(nodeList[i - 1].toVector()[0] == "if"
        || nodeList[i - 1].toVector()[0] == "else") {
        continue;
      } else {
        throw RenderException("Unexcpected 'endif' node at line " + std::to_string(lineNum));
      }
    } else {
      throw RenderException("Invalid node '" + nodeVector[0] + "' at line " + std::to_string(lineNum));
    }
  }

  /* If no node, we assume susbstitution of all code for vars */
  if(nodeList.size() == 0)
    localCode = sourceCode;

  /* Find and process vars */
  for(size_t varPos = 0; (varPos = localCode.find("(~", varPos)) != std::string::npos; ++varPos) {
    /* Find the closing marker for the var */
    size_t varEnd = localCode.find(")", varPos) + 1;

    /* Calculate line number */
    size_t lineNum = std::count(localCode.begin(), localCode.begin() + varPos, '\n') + 1;

    /* Throw an error if the var is not closed on the same line */
    if (localCode.find(")", varPos) == std::string::npos || localCode.find("\n",  varPos) < varEnd) {
      throw RenderException("Cannot find matching closing marker for the opened var at line " + std::to_string(lineNum));
    }

    /* Get var name & value */
    std::string varName = localCode.substr((varPos + 2), (varEnd - 1) - (varPos + 2));
    std::string varVal = valueByName[varName]->toString();

    /* Do substitution */
    localCode.replace(varPos, varEnd - varPos, varVal);
  }

  return localCode;
}
