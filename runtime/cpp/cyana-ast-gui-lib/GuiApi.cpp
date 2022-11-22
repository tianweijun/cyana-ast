#include "GuiApi.h"

#include <QApplication>

#include "MainWindow.h"

int guiapi::showViewUntilClose(StringTree *strTree) {
  int argc = 0;
  char *argv[] = {""};
  QApplication app(argc, argv);
  MainWindow w(strTree, 0);
  w.app = &app;
  w.show();

  int exitCode = app.exec();

  return exitCode;
}
