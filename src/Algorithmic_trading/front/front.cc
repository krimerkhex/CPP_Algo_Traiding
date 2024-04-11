#include "front.h"

MainWindow::MainWindow(s21::Model &model, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), model_(model) {
  ui_->setupUi(this);
  grafA_ = 0;
  grafI_ = 0;
  setSetingToWindow(*ui_->InterpolationGraphView);
  setSetingToWindow(*ui_->ApproximationGraphView);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::setSetingToWindow(QCustomPlot &window) {
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("dd MM yyyy");
  window.xAxis->setTicker(dateTicker);
  window.xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  window.yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  window.setInteraction(QCP::iRangeZoom, true);
  window.setInteraction(QCP::iRangeDrag, true);
  window.axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  window.axisRect()->setRangeZoom(Qt::Horizontal);
  window.legend->setVisible(true);
  window.legend->setBrush(QColor(150, 150, 150, 150));
  window.axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignLeft | Qt::AlignTop);
  window.xAxis->setLabel("Date");
  window.yAxis->setLabel("Price");
  window.xAxis2->setVisible(true);
  window.yAxis2->setVisible(true);
  window.xAxis2->setTicks(false);
  window.yAxis2->setTicks(false);
  window.xAxis2->setTickLabels(false);
  window.yAxis2->setTickLabels(false);
}

QVector<double> fromMatrixToVector(s21::Matrix &data) {
  QVector<double> ret_value;
  for (unsigned int i = 0; i < data.getRow(); ++i) {
    for (unsigned int j = 0; j < data.getCol(); ++j) {
      ret_value.push_back(data(i, j));
    }
  }
  return ret_value;
}

void MainWindow::drawingGraphAlgoritm(QCustomPlot &window,
                                      QVector<QCPGraphData> &timeSeries,
                                      QVector<QCPGraph *> &graf,
                                      QString grafName) {
  graf.push_back(window.addGraph());
  window.graph()->setLineStyle(QCPGraph::lsLine);
  window.graph()->setPen(
      QPen(QColor((rand() % 255 + 50) % 255, (rand() % 255 + 50) % 255,
                  (rand() % 255 + 50) % 255)));
  window.xAxis->setRange(timeSeries[0].key - 86400,
                         timeSeries.last().key + 86400);
  window.graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
  window.graph()->data()->set(timeSeries);
  window.graph()->setName(grafName);
  window.rescaleAxes();
  window.replot();
}

void MainWindow::clearGraf(QVector<QCPGraph *> &graf, QCustomPlot &window) {
  window.clearGraphs();
  graf.clear();
}

void MainWindow::loadDataOnFile(std::string algoritm) {
  std::string patch =
      QFileDialog::getOpenFileName(this, "File selection", "", "*.csv")
          .toStdString();
  if (!patch.size() || model_.parsFile(patch, algoritm)) {
    QMessageBox::warning(this, "Warning",
                         "Failed to read the file, "
                         "please check the correctness "
                         "of the data in the file.");
  } else {
    if (algoritm == "least squares") {
      ui_->ApproxSourceFile->setText(QString::fromStdString(patch));
      ui_->DateForPromotionApproximation->setText("YYYY-MM-DD");
      ui_->ApproxPointsForPlotting->setValue(0);
      ui_->ApproxCoefficient->setValue(0);
      ui_->ApproxDayPred->setValue(0);
      ui_->ApproxPromotionValue->setText("");
      on_ApproxClearGraphBut_clicked();
    } else {
      ui_->InterSourceFile->setText(QString::fromStdString(patch));
      ui_->DateForPromotionInterpolation->setText("YYYY-MM-DD");
      ui_->InterPointsForPlotting->setValue(0);
      ui_->InterPromotionValue->setText("");
      on_InterClearGraphBut_clicked();
    }
  }
}

void MainWindow::on_ApproxInterLoadDataBut_clicked() {
  loadDataOnFile("least squares");
}

void MainWindow::on_InterLoadDataBut_clicked() {
  loadDataOnFile("cubic spline");
}

void MainWindow::plotGrafAlgoritm(std::string algoritm) {
  if ((algoritm == "cubic spline" && model_.getDataInterpolation().size()) ||
      (algoritm == "least squares" && model_.getDataApproximation().size())) {
    model_.setPolinomPower(ui_->ApproxCoefficient->value());
    bool error = model_.startAlgoritm(
        algoritm,
        algoritm == "cubic spline" ? ui_->InterPointsForPlotting->value()
                                   : ui_->ApproxPointsForPlotting->value(),
        ui_->ApproxDayPred->value());
    if (algoritm == "cubic spline" && error) {
      QMessageBox::warning(this, "Warning",
                           "The number of points for construction cannot be "
                           "less than the amount of data in the file");
    } else if ((algoritm == "least squares" && error) ||
               (ui_->ApproxCoefficient->value() > 5 ||
                ui_->ApproxCoefficient->value() < 0)) {
      QMessageBox::warning(this, "Warning",
                           "When using the least squares method, "
                           "a solution may not be found."
                           "The number of points for construction cannot be "
                           "less than the amount of data in the file");
    } else {
      if (algoritm == "cubic spline") {
        if (grafI_ < 5) {
          grafI_++;
          QString name = "schedule number " + QString::number(grafI_) + "\n" +
                         "Number of points in the graph " +
                         QString::number(ui_->InterPointsForPlotting->value());
          drawingGraphAlgoritm(*ui_->InterpolationGraphView,
                               model_.getTimeSeriesSpline(), graphsI_, name);
        }
      } else {
        if (grafA_ < 5) {
          grafA_++;
          QString name = "schedule number " + QString::number(grafA_) + "\n" +
                         "Number of points in the graph " +
                         QString::number(ui_->ApproxPointsForPlotting->value());
          drawingGraphAlgoritm(*ui_->ApproximationGraphView,
                               model_.getTimeSeriesLeastSquares(), graphsA_,
                               name);
        }
      }
    }
  }
}

void MainWindow::on_InterPlotGrafBut_clicked() {
  plotGrafAlgoritm("cubic spline");
}

void MainWindow::on_ApproxPlotGrafBut_clicked() {
  plotGrafAlgoritm("least squares");
}

void MainWindow::on_ApproxClearGraphBut_clicked() {
  if (model_.getDataApproximation().size()) {
    grafA_ = 0;
    clearGraf(graphsA_, *ui_->ApproximationGraphView);
    drawingGraphAlgoritm(*ui_->ApproximationGraphView,
                         model_.getDataApproximation(), graphsA_,
                         "Initial data");
  }
}

void MainWindow::on_InterClearGraphBut_clicked() {
  if (model_.getDataInterpolation().size()) {
    grafI_ = 0;
    clearGraf(graphsI_, *ui_->InterpolationGraphView);
    drawingGraphAlgoritm(*ui_->InterpolationGraphView,
                         model_.getDataInterpolation(), graphsI_,
                         "Initial data");
  }
}

void MainWindow::on_DateForPromotionApproximation_textChanged(
    const QString &arg1) {
  if (!model_.checkDateFormat(arg1.toStdString()) &&
      model_.getDataApproximation().size() &&
      ui_->ApproxCoefficient->value() <= 5 &&
      ui_->ApproxCoefficient->value() >= 0) {
    model_.setPolinomPower(ui_->ApproxCoefficient->value());
    QDateTime date = model_.dateToQDate(arg1);
    ui_->ApproxPromotionValue->setText(
        QString::number(model_.getDateLeastSquares(date)));
  } else {
    ui_->ApproxPromotionValue->setText("0");
  }
}

void MainWindow::on_DateForPromotionInterpolation_textChanged(
    const QString &arg1) {
  if (!model_.checkDateFormat(arg1.toStdString()) &&
      model_.getDataInterpolation().size()) {
    QDateTime date = model_.dateToQDate(arg1);
    if (static_cast<double>(date.toSecsSinceEpoch()) >=
            model_.getDataInterpolation().front().key &&
        static_cast<double>(date.toSecsSinceEpoch()) <=
            model_.getDataInterpolation().back().key) {
      ui_->InterPromotionValue->setText(
          QString::number(model_.getDateSpline(date)));
    }
  } else {
    ui_->InterPromotionValue->setText("0");
  }
}

void MainWindow::on_ApproxCoefficient_valueChanged() {
  on_DateForPromotionApproximation_textChanged(
      ui_->DateForPromotionApproximation->text());
}

void MainWindow::on_ApproxPointsForPlotting_valueChanged(int arg1) {
  if (arg1 < 2) {
    ui_->ApproxDayPred->setMaximum(0);
  } else {
    ui_->ApproxDayPred->setMaximum(arg1 - 2);
  }
}
