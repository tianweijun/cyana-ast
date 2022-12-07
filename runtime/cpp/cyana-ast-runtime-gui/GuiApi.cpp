#include "GuiApi.h"

#include <QApplication>

#include "MainWindow.h"

int guiapi::showViewUntilClose(const StringTree *strTree) {
  if (0 == strTree) {
    return 0;
  }
  int argc = 0;
  char *argv[] = {0};
  QApplication app(argc, argv);
  MainWindow w(strTree, 0);
  w.app = &app;
  w.show();

  int exitCode = app.exec();

  return exitCode;
}
