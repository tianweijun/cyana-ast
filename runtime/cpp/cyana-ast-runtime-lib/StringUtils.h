//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST__STRINGUTILS_H_
#define CYANA_AST__STRINGUTILS_H_

#include <string>

class StringUtils {
 private:
  StringUtils() = default;

 public:
  StringUtils(StringUtils &stringUtils) = delete;
  StringUtils(StringUtils &&stringUtils) = delete;
  static bool isEmpty(const std::string *str);
  static bool isNotEmpty(const std::string *str);
  static bool isBlank(const std::string *str);
  static bool isNotBlank(const std::string *str);
};

#endif//CYANA_AST__STRINGUTILS_H_
