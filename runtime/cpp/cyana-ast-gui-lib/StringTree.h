#ifndef STRINGTREE_H
#define STRINGTREE_H
#include <list>
#include <string>

#include "cyana-ast-gui-lib_global.h"

class CYANAASTGUILIBSHARED_EXPORT StringTree {
 public:
  StringTree();
  ~StringTree();
  StringTree(StringTree* parent);
  StringTree* parent;
  std::list<StringTree*>* children;
  std::string* text;
};

#endif  // STRINGTREE_H
