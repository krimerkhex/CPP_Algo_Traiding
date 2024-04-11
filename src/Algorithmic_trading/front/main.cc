#include <QApplication>

#include "front.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Model &model = s21::Model::getInstanse();
  MainWindow w(model);
  w.show();
  return a.exec();
}
