#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "algorithm.h"
#include "parser.h"

namespace s21 {
class Model {
 public:
  static Model& getInstanse();
  ~Model();

  Model(const Model&) = delete;
  Model(Model&&) = delete;
  Model(const Model&&) = delete;

  bool parsFile(const std::string& fileName, std::string& algoritm);
  bool startAlgoritm(const std::string& algoritm,
                     unsigned int numberOfPoints = 0,
                     unsigned int dayPredict = 0);

  QVector<QCPGraphData>& getDataApproximation();
  QVector<QCPGraphData>& getDataInterpolation();
  QVector<QCPGraphData>& getTimeSeriesSpline();
  QVector<QCPGraphData>& getTimeSeriesLeastSquares();

  QDateTime dateToQDate(const QString& str);

  double getDateSpline(QDateTime date);
  double getDateLeastSquares(QDateTime date);

  void setPolinomPower(unsigned int polinomPower);

  bool checkDateFormat(const std::string& input);

 private:
  Model();

  QVector<QCPGraphData> dataApproximation_;

  QVector<QCPGraphData> dataInterpolation_;

  QVector<QCPGraphData> timeSeriesSpline_;
  s21::Matrix splineCoefficients_;

  QVector<QCPGraphData> timeSeriesLeastSquares_;
  s21::Matrix leastSquaresCoefficients_;

  unsigned int polinomPower_;
};
}  // namespace s21

#endif  // MODEL_H
