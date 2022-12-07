#include "StringTree.h"

StringTree::StringTree() : parent(0), text(0) {
  children = new std::list<StringTree*>();
}

StringTree::StringTree(StringTree* parent) : StringTree() {
  this->parent = parent;
}

StringTree::~StringTree() {
  delete text;
  text = 0;
  // delete children
  if (children != 0) {
    for (std::list<StringTree*>::const_iterator strTreeChildrenIt =
             children->begin();
         strTreeChildrenIt != children->end();) {
      StringTree* strTreeChild = *strTreeChildrenIt;
      delete strTreeChild;
      strTreeChild = 0;
      strTreeChildrenIt = children->erase(strTreeChildrenIt);
    }
    delete children;
    children = 0;
  }
}
