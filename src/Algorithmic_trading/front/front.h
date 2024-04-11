#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <ctime>
#include <sstream>

#include "../back/model.h"
#include "../ui_front.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Model &model, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_ApproxInterLoadDataBut_clicked();
  void on_InterLoadDataBut_clicked();
  void on_InterPlotGrafBut_clicked();
  void on_ApproxPlotGrafBut_clicked();
  void on_ApproxClearGraphBut_clicked();
  void on_InterClearGraphBut_clicked();
  void on_DateForPromotionApproximation_textChanged(const QString &arg1);
  void on_DateForPromotionInterpolation_textChanged(const QString &arg1);
  void on_ApproxCoefficient_valueChanged();

  void on_ApproxPointsForPlotting_valueChanged(int arg1);

 private:
  void setSetingToWindow(QCustomPlot &window);
  void clearGraf(QVector<QCPGraph *> &graf, QCustomPlot &window);
  void loadDataOnFile(std::string algoritm);
  void plotGrafAlgoritm(std::string algoritm);
  void drawingGraphAlgoritm(QCustomPlot &window,
                            QVector<QCPGraphData> &timeSeries,
                            QVector<QCPGraph *> &graf, QString grafName);
  Ui::MainWindow *ui_;
  s21::Model &model_;
  short grafA_, grafI_;
  QVector<QCPGraph *> graphsA_, graphsI_;
};
#endif  // MAINWINDOW_H
