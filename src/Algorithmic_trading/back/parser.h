#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include "../contain/matrix.h"
#include "../qcustomplot.h"

namespace s21 {
class Parser {
 public:
  static bool parsFile(const std::string& fileName,
                       QVector<QCPGraphData>& data);
  static bool checkDateFormat(const std::string& input);

 private:
  static bool checkFile(std::ifstream& fileStream, unsigned int& lenFile);
  static bool checkFileLine(const std::string& line);
  static bool checkStringFormat(const std::string& input);
  static void getDataOnFile(QVector<QCPGraphData>& data,
                            std::ifstream& fileStream);
};
}  // namespace s21

#endif  // PARSER_H
