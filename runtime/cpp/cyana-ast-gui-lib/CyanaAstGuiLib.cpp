#include "CyanaAstGuiLib.h"

#include "GuiApi.h"

CyanaAstGuiLib::CyanaAstGuiLib() {}

int CyanaAstGuiLib::showViewUntilClose(StringTree *strTree) {
  guiapi::showViewUntilClose(strTree);
}
