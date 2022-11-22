#ifndef CYANAASTGUILIB_H
#define CYANAASTGUILIB_H

#include "StringTree.h"
#include "cyana-ast-gui-lib_global.h"

class CYANAASTGUILIBSHARED_EXPORT CyanaAstGuiLib {
 public:
  CyanaAstGuiLib();

 public:
  static int showViewUntilClose(StringTree *strTree);
};

#endif  // CYANAASTGUILIB_H
