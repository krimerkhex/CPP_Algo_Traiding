#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <math.h>

#include <ctime>
#include <string>
#include <vector>

#include "../contain/matrix.h"
#include "../qcustomplot.h"

namespace s21 {
class Algorithm {
 public:
  /*
   * the cubic spline method
   */
  static void theCubicSplineMethod(s21::Matrix& splineCoefficients,
                                   QVector<QCPGraphData>& timeSeries);
  static void buildingSplineForTheNumberOfPoints(
      QVector<QCPGraphData>& timeSeriesSpline, s21::Matrix& splineCoefficients,
      QVector<QCPGraphData>& timeSeries, unsigned int numberOfPoints);
  static double splineDate(s21::Matrix& splineCoefficients,
                           QVector<QCPGraphData>& timeSeries,
                           QDateTime& dateTime);
  /*
   * the least squares method
   */
  static bool theLeastSquaresMethod(s21::Matrix& leastSquaresCoefficients,
                                    QVector<QCPGraphData>& timeSeries,
                                    unsigned int polinomPower);
  static void buildingTimeSeriesLeastSquares(
      unsigned int numberOfPoints, unsigned int dayPredict,
      QVector<QCPGraphData>& timeSeriesLeastSquares,
      QVector<QCPGraphData>& dataApproximation,
      s21::Matrix& leastSquaresCoefficients);
  static double leastSquaresDate(s21::Matrix& leastSquaresCoefficients,
                                 QDateTime& dateTime);

 private:
  /*
   * the cubic spline method
   */
  static void calculateCoefficients(s21::Matrix& splineCoefficients,
                                    QVector<QCPGraphData>& timeSeries);
  static void searchForTheSecondCoefficient(
      s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference);
  static void searchForTheThirdCoefficient(
      s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference);
  static void searchForTheFourthCoefficient(
      s21::Matrix& splineCoefficients, QVector<QCPGraphData>& pointDifference);
  static s21::Matrix initializationOfTheCoefficientCalculationMatrix(
      QVector<QCPGraphData>& pointDifference);
  /*
   * the least squares method
   */
  static bool calcCoefficientsLeastSquares(
      s21::Matrix& leastSquaresCoefficients_, s21::Matrix& squareSumsMatrix,
      s21::Matrix& freeCoefficientsColumn);
  static void initSquareSumsMatrix(QVector<QCPGraphData>& timeSeries,
                                   s21::Matrix& squareSumsMatrix);
  static void initFreeCoefficientsColumn(QVector<QCPGraphData>& timeSeries,
                                         s21::Matrix& freeCoefficientsColumn);
  static void matrixReductionToDiagonalMatrix(
      s21::Matrix& squareSumsMatrix, s21::Matrix& freeCoefficientsColumn);
};
}  // namespace s21

#endif  // ALGORITHM_H
