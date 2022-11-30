//
// for ascll
// Created by tian wei jun on 2022/11/24 0024.
//
//

#include "StringUtils.h"

bool StringUtils::isEmpty(std::string &str) {
  return &str == 0 || str.length() <= 0;
}

bool StringUtils::isNotEmpty(std::string &str) {
  return !isEmpty(str);
}

bool StringUtils::isBlank(std::string &str) {
  if (&str == 0 || str.length() <= 0) {
    return true;
  }
  bool isBlank = true;
  const char *chars = str.c_str();
  const char *charIt = chars;
  while (isBlank && *charIt != '\0') {
    char tmpChar = *charIt;
    switch (tmpChar) {// {32, 9, 13}
      case 32:
      case 9:
      case 13:
        break;
      default:
        isBlank = false;
    }
    ++tmpChar;
  }
  return isBlank;
}

bool StringUtils::isNotBlank(std::string &str) {
  return !isBlank(str);
}