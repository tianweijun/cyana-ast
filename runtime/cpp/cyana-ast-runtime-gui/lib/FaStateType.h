//
// Created by tian wei jun on 2022/12/1 0001.
//

#ifndef CYANA_AST__FASTATETYPE_H_
#define CYANA_AST__FASTATETYPE_H_

enum class  FaStateEnumType : int {
  NONE = 0,
  NORMAL = 1,
  OPENING_TAG = 2,
  CLOSING_TAG = 4
};

class __declspec(dllimport) FaStateType {
 public:
  static int appendState(int state, FaStateEnumType appendState) {
    return state | (int) appendState;
  }
  static int removeState(int state, FaStateEnumType removeState) {
    return state & (~(int) removeState);
  }
  static bool isClosingTag(int state) {
    return (state & (int) FaStateEnumType::CLOSING_TAG) != 0;
  }
};

#endif//CYANA_AST__FASTATETYPE_H_
