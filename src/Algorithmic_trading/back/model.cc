#include "model.h"

s21::Model::Model()
    : dataApproximation_(QVector<QCPGraphData>()),
      dataInterpolation_(QVector<QCPGraphData>()),
      timeSeriesSpline_(QVector<QCPGraphData>()),
      splineCoefficients_(s21::Matrix()),
      timeSeriesLeastSquares_(QVector<QCPGraphData>()),
      leastSquaresCoefficients_(s21::Matrix()) {
  polinomPower_ = 0;
}

s21::Model::~Model() {}

s21::Model& s21::Model::getInstanse() {
  static Model model;
  return model;
}

bool s21::Model::parsFile(const std::string& fileName, std::string& algoritm) {
  bool error = false;
  if (algoritm == "cubic spline") {
    dataInterpolation_.clear();
    error = s21::Parser::parsFile(fileName, dataInterpolation_);
    if (!error) {
      s21::Algorithm::theCubicSplineMethod(splineCoefficients_,
                                           dataInterpolation_);
    }
  } else if (algoritm == "least squares") {
    dataApproximation_.clear();
    error = s21::Parser::parsFile(fileName, dataApproximation_);
  }
  return error;
}

bool s21::Model::startAlgoritm(const std::string& algoritm,
                               unsigned int numberOfPoints,
                               unsigned int dayPredict) {
  bool error = false;
  if (algoritm == "cubic spline") {
    if (dataInterpolation_.size() > numberOfPoints) {
      error = true;
    } else {
      timeSeriesSpline_.clear();
      s21::Algorithm::buildingSplineForTheNumberOfPoints(
          timeSeriesSpline_, splineCoefficients_, dataInterpolation_,
          numberOfPoints);
    }
  } else if (algoritm == "least squares") {
    if (dataApproximation_.size() > numberOfPoints) {
      error = true;
    } else {
      error = s21::Algorithm::theLeastSquaresMethod(
          leastSquaresCoefficients_, dataApproximation_, polinomPower_);
      timeSeriesLeastSquares_.clear();
      s21::Algorithm::buildingTimeSeriesLeastSquares(
          numberOfPoints, dayPredict, timeSeriesLeastSquares_,
          dataApproximation_, leastSquaresCoefficients_);
    }
  }
  return error;
}

double s21::Model::getDateLeastSquares(QDateTime date) {
  s21::Algorithm::theLeastSquaresMethod(leastSquaresCoefficients_,
                                        dataApproximation_, polinomPower_);
  return s21::Algorithm::leastSquaresDate(leastSquaresCoefficients_, date);
}

double s21::Model::getDateSpline(QDateTime date) {
  return s21::Algorithm::splineDate(splineCoefficients_, dataInterpolation_,
                                    date);
}

void s21::Model::setPolinomPower(unsigned int polinomPower) {
  polinomPower_ = polinomPower;
}

QVector<QCPGraphData>& s21::Model::getDataApproximation() {
  return dataApproximation_;
}

QVector<QCPGraphData>& s21::Model::getDataInterpolation() {
  return dataInterpolation_;
}

QVector<QCPGraphData>& s21::Model::getTimeSeriesSpline() {
  return timeSeriesSpline_;
}

QVector<QCPGraphData>& s21::Model::getTimeSeriesLeastSquares() {
  return timeSeriesLeastSquares_;
}

bool s21::Model::checkDateFormat(const std::string& input) {
  bool res = s21::Parser::checkDateFormat(input);

  return res;
}

QDateTime s21::Model::dateToQDate(const QString& str) {
  short year = std::stoi(str.toStdString().substr(0, 4));
  short mon = std::stoi(str.toStdString().substr(5, 2));
  short day = std::stoi(str.toStdString().substr(8, 2));
  return QDateTime(QDate(year, mon, day), QTime());
}
