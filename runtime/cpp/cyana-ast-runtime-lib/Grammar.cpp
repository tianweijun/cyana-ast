//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Grammar.h"

Grammar::Grammar(const std::string *name, const GrammarType type, const GrammarAction action) : name(name), type(type), action(action) {
}

//name delete by PersistentData.stringPool
Grammar::~Grammar() = default;
