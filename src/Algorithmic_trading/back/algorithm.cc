#include "algorithm.h"

/*
 * the cubic spline method
 */

void s21::Algorithm::theCubicSplineMethod(s21::Matrix& splineCoefficients,
                                          QVector<QCPGraphData>& timeSeries) {
  splineCoefficients.reSize(timeSeries.size() - 1, 4);
  calculateCoefficients(splineCoefficients, timeSeries);
}

void s21::Algorithm::calculateCoefficients(s21::Matrix& splineCoefficients,
                                           QVector<QCPGraphData>& timeSeries) {
  for (unsigned int i = 0; i < timeSeries.size() - 1; ++i) {
    splineCoefficients(i, 0) = timeSeries[i].value;
  }
  QVector<QCPGraphData> pointDifference(timeSeries.size() - 1);
  for (unsigned int i = 0; i < timeSeries.size() - 1; ++i) {
    pointDifference[i].value = timeSeries[i + 1].value - timeSeries[i].value;
    pointDifference[i].key = timeSeries[i + 1].key - timeSeries[i].key;
  }
  searchForTheSecondCoefficient(splineCoefficients, pointDifference);
  searchForTheThirdCoefficient(splineCoefficients, pointDifference);
  searchForTheFourthCoefficient(splineCoefficients, pointDifference);
}

void s21::Algorithm::searchForTheThirdCoefficient(
    s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference) {
  for (unsigned int i = 0; i < splineCoefficients.getRow() - 1; ++i) {
    splineCoefficients(i, 3) =
        (splineCoefficients(i + 1, 2) - splineCoefficients(i, 2)) /
        (3 * pointDifference[i].key);
  }
  splineCoefficients(splineCoefficients.getRow() - 1, 3) =
      -splineCoefficients(splineCoefficients.getRow() - 1, 2) /
      (3 * pointDifference.last().key);
}

void s21::Algorithm::searchForTheFourthCoefficient(
    s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference) {
  for (unsigned int i = 0; i < splineCoefficients.getRow(); ++i) {
    splineCoefficients(i, 1) =
        pointDifference[i].value / pointDifference[i].key -
        pointDifference[i].key * splineCoefficients(i, 2) -
        pow(pointDifference[i].key, 2) * splineCoefficients(i, 3);
  }
}

void s21::Algorithm::searchForTheSecondCoefficient(
    s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference) {
  s21::Matrix coefficientCalculationMatrix(
      initializationOfTheCoefficientCalculationMatrix(pointDifference));
  splineCoefficients(splineCoefficients.getRow() - 1, 2) =
      coefficientCalculationMatrix(coefficientCalculationMatrix.getRow() - 1,
                                   3) /
      coefficientCalculationMatrix(coefficientCalculationMatrix.getRow() - 1,
                                   1);
  for (int i = coefficientCalculationMatrix.getRow() - 2; i >= 0; --i) {
    splineCoefficients(i, 2) =
        (coefficientCalculationMatrix(i, 3) -
         coefficientCalculationMatrix(i, 2) * splineCoefficients(i + 1, 2)) /
        coefficientCalculationMatrix(i, 1);
  }
}

s21::Matrix s21::Algorithm::initializationOfTheCoefficientCalculationMatrix(
    QVector<QCPGraphData>& pointDifference) {
  double x;
  s21::Matrix res(pointDifference.size(), 4);
  res(0, 1) = 1.;
  for (unsigned int i = 1; i < pointDifference.size(); ++i) {
    res(i, 0) = pointDifference[i - 1].key;
    res(i, 1) = 2. * (pointDifference[i - 1].key + pointDifference[i].key);
    res(i, 2) = pointDifference[i].key;
    res(i, 3) = 3 * (pointDifference[i].value / pointDifference[i].key -
                     pointDifference[i - 1].value / pointDifference[i - 1].key);
  }
  res(res.getRow() - 1, 2) = 0.;
  for (unsigned int i = 1; i < res.getRow(); i++) {
    x = res(i, 0) / res(i - 1, 1);
    res(i, 1) -= x * res(i - 1, 2);
    res(i, 3) -= x * res(i - 1, 3);
  }
  return res;
}

void s21::Algorithm::buildingSplineForTheNumberOfPoints(
    QVector<QCPGraphData>& timeSeriesSpline, s21::Matrix& splineCoefficients,
    QVector<QCPGraphData>& timeSeries, unsigned int numberOfPoints) {
  timeSeriesSpline = QVector<QCPGraphData>(numberOfPoints);
  double left = timeSeries[0].key;
  double right = timeSeries.last().key;
  double step = (right - left) / (numberOfPoints - 1);
  unsigned int j = 0;
  for (unsigned int i = 0; i < numberOfPoints - 1; ++i) {
    double stepPoz = left + step * i;
    while (stepPoz > timeSeries[j + 1].key) j++;
    timeSeriesSpline[i].key = stepPoz;
    for (short cofPoz = 0; cofPoz < 4; ++cofPoz) {
      timeSeriesSpline[i].value += splineCoefficients(j, cofPoz) *
                                   pow((stepPoz - timeSeries[j].key), cofPoz);
    }
  }
  timeSeriesSpline[timeSeriesSpline.size() - 1].key = right;
  timeSeriesSpline[timeSeriesSpline.size() - 1].value =
      timeSeries[timeSeries.size() - 1].value;
}

double s21::Algorithm::splineDate(s21::Matrix& splineCoefficients,
                                  QVector<QCPGraphData>& timeSeries,
                                  QDateTime& dateTime) {
  double res = 0;
  double stepPoz = static_cast<double>(dateTime.toSecsSinceEpoch());
  int j = 0;
  while (stepPoz > timeSeries[j + 1].key) j++;
  for (short cofPoz = 0; cofPoz < 4; cofPoz++) {
    res += splineCoefficients(j, cofPoz) *
           pow((stepPoz - timeSeries[j].key), cofPoz);
  }
  return res;
}

/*
 * the least squares method
 */

void s21::Algorithm::buildingTimeSeriesLeastSquares(
    unsigned int numberOfPoints, unsigned int dayPredict,
    QVector<QCPGraphData>& timeSeriesLeastSquares,
    QVector<QCPGraphData>& dataApproximation,
    s21::Matrix& leastSquaresCoefficients) {
  timeSeriesLeastSquares = QVector<QCPGraphData>(numberOfPoints + dayPredict);
  double left = dataApproximation[0].key;
  double right = dataApproximation[dataApproximation.size() - 1].key;
  double step = (right - left) / (numberOfPoints - dayPredict - 1);
  for (unsigned int i = 0; i < timeSeriesLeastSquares.size(); i++) {
    double stepPoz = left + step * i;
    timeSeriesLeastSquares[i].key = stepPoz;
    for (unsigned int k = 0; k < leastSquaresCoefficients.getCol(); k++) {
      timeSeriesLeastSquares[i].value +=
          leastSquaresCoefficients(0, k) * pow(stepPoz, k);
    }
  }
}

double s21::Algorithm::leastSquaresDate(s21::Matrix& leastSquaresCoefficients,
                                        QDateTime& dateTimes) {
  double stepPoz = static_cast<double>(dateTimes.toSecsSinceEpoch());
  double res = 0;
  for (unsigned int k = 0; k < leastSquaresCoefficients.getCol(); k++) {
    res += leastSquaresCoefficients(0, k) * pow(stepPoz, k);
  }
  return res;
}

bool s21::Algorithm::theLeastSquaresMethod(
    s21::Matrix& leastSquaresCoefficients, QVector<QCPGraphData>& timeSeries,
    unsigned int polinomPower) {
  s21::Matrix squareSumsMatrix(polinomPower + 1, polinomPower + 1);
  s21::Matrix freeCoefficientsColumn(1, polinomPower + 1);
  leastSquaresCoefficients.reSize(1, polinomPower + 1);
  initSquareSumsMatrix(timeSeries, squareSumsMatrix);
  initFreeCoefficientsColumn(timeSeries, freeCoefficientsColumn);
  matrixReductionToDiagonalMatrix(squareSumsMatrix, freeCoefficientsColumn);
  return calcCoefficientsLeastSquares(leastSquaresCoefficients,
                                      squareSumsMatrix, freeCoefficientsColumn);
}

bool s21::Algorithm::calcCoefficientsLeastSquares(
    s21::Matrix& leastSquaresCoefficients, s21::Matrix& squareSumsMatrix,
    s21::Matrix& freeCoefficientsColumn) {
  bool error = false;
  for (unsigned int k = 0; k < freeCoefficientsColumn.getCol() && !error; k++) {
    for (unsigned int i = k + 1; i < freeCoefficientsColumn.getCol() && !error;
         i++) {
      if (squareSumsMatrix(k, k) == 0) {
        error = true;
        break;
      }
      double M = squareSumsMatrix(i, k) / squareSumsMatrix(k, k);
      for (unsigned int j = k; j < freeCoefficientsColumn.getCol(); j++) {
        squareSumsMatrix(i, j) -= M * squareSumsMatrix(k, j);
      }
      freeCoefficientsColumn(0, i) -= M * freeCoefficientsColumn(0, k);
    }
  }
  if (!error) {
    for (int i = freeCoefficientsColumn.getCol() - 1; i >= 0; i--) {
      float s = 0;
      for (unsigned int j = i; j < freeCoefficientsColumn.getCol(); j++) {
        s = s + squareSumsMatrix(i, j) * leastSquaresCoefficients(0, j);
      }
      leastSquaresCoefficients(0, i) =
          (freeCoefficientsColumn(0, i) - s) / squareSumsMatrix(i, i);
    }
  }
  return error;
}

void s21::Algorithm::initSquareSumsMatrix(QVector<QCPGraphData>& timeSeries,
                                          s21::Matrix& squareSumsMatrix) {
  for (unsigned int i = 0; i < squareSumsMatrix.getRow(); i++) {
    for (unsigned int j = 0; j < squareSumsMatrix.getCol(); j++) {
      for (unsigned int k = 0; k < timeSeries.size(); k++) {
        squareSumsMatrix(i, j) += pow(timeSeries[k].key, i + j);
      }
    }
  }
}

void s21::Algorithm::initFreeCoefficientsColumn(
    QVector<QCPGraphData>& timeSeries, s21::Matrix& freeCoefficientsColumn) {
  for (unsigned int i = 0; i < freeCoefficientsColumn.getCol(); i++) {
    for (unsigned int k = 0; k < timeSeries.size(); k++) {
      freeCoefficientsColumn(0, i) +=
          pow(timeSeries[k].key, i) * timeSeries[k].value;
    }
  }
}

void s21::Algorithm::matrixReductionToDiagonalMatrix(
    s21::Matrix& squareSumsMatrix, s21::Matrix& freeCoefficientsColumn) {
  double temp = 0;
  for (unsigned int i = 0; i < squareSumsMatrix.getCol(); i++) {
    if (squareSumsMatrix(i, i) != 0) continue;
    for (unsigned int j = 0; j < squareSumsMatrix.getCol(); j++) {
      if (i == j || squareSumsMatrix(j, i) == 0 || squareSumsMatrix(i, j) == 0)
        continue;
      for (unsigned int k = 0; k < squareSumsMatrix.getCol(); k++) {
        temp = squareSumsMatrix(j, k);
        squareSumsMatrix(j, k) = squareSumsMatrix(i, k);
        squareSumsMatrix(i, k) = temp;
      }
      temp = freeCoefficientsColumn(0, j);
      freeCoefficientsColumn(0, j) = freeCoefficientsColumn(0, i);
      freeCoefficientsColumn(0, i) = temp;
      break;
    }
  }
}
