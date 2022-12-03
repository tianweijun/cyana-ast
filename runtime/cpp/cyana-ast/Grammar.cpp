//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Grammar.h"

Grammar::Grammar(std::string *name) : name(name), type(GrammarType::TERMINAL), action(GrammarAction::TEXT) {
}

Grammar::~Grammar() {
  //name delete by PersistentData.stringPool
}
