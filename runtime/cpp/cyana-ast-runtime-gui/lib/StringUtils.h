//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST__STRINGUTILS_H_
#define CYANA_AST__STRINGUTILS_H_

#include <stdarg.h>
#include <string>

class __declspec(dllimport) StringUtils {
 public:
  static bool isEmpty(std::string *str);
  static bool isNotEmpty(std::string *str);
  static bool isBlank(std::string *str);
  static bool isNotBlank(std::string *str);
};

#endif//CYANA_AST__STRINGUTILS_H_
