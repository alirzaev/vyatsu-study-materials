#include <cstdlib>
#include <ctime>

#include <QApplication>

#include "widget.h"

int main(int argc, char *argv[]) {
  //    srand(time(nullptr));
  srand(0);

  QApplication a(argc, argv);
  Widget w;
  w.show();

  return a.exec();
}
