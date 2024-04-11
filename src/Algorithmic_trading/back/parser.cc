#include "parser.h"

bool s21::Parser::parsFile(const std::string& fileName,
                           QVector<QCPGraphData>& data) {
  std::ifstream file(fileName);
  unsigned int lenFile = 0;
  if (!file.is_open() || checkFile(file, lenFile)) {
    return true;
  }
  getDataOnFile(data, file);
  file.close();
  return false;
}

void s21::Parser::getDataOnFile(QVector<QCPGraphData>& data,
                                std::ifstream& fileStream) {
  std::string line;
  getline(fileStream, line);
  while (getline(fileStream, line)) {
    short year = std::stoi(line.substr(0, 4));
    short mon = std::stoi(line.substr(5, 2));
    short day = std::stoi(line.substr(8, 2));
    unsigned int i = 0;
    std::string tmp;
    for (; line[i] != ','; i++)
      ;
    i++;
    for (; i < line.size(); i++) {
      tmp += line[i];
    }
    double value = std::stod(tmp);
    data.push_back(QCPGraphData(
        static_cast<double>(
            QDateTime(QDate(year, mon, day), QTime()).toSecsSinceEpoch()),
        value));
  }
}

bool s21::Parser::checkFile(std::ifstream& fileStream, unsigned int& lenFile) {
  bool error = false;
  std::string line;
  getline(fileStream, line);
  if (line.back() != 'e') line.pop_back();
  if (line != "Date,Close") {
    error = true;
  }
  while (!error && getline(fileStream, line)) {
    lenFile++;
    if (line.back() < '0' || line.back() > '9') line.pop_back();
    if (checkFileLine(line)) {
      error = true;
      lenFile = 0;
    }
  }
  fileStream.clear();
  fileStream.seekg(0);
  return error;
}

bool s21::Parser::checkFileLine(const std::string& line) {
  return checkStringFormat(line);
}

bool s21::Parser::checkStringFormat(const std::string& input) {
  std::regex pattern(
      R"((\d{4})-(([0][1-9])|([1][012]))-(([0][1-9])|([12][0-9])|([3][012])),-*[0-9]+.[0-9]+)");
  return !std::regex_match(input, pattern);
}

bool s21::Parser::checkDateFormat(const std::string& input) {
  std::regex pattern(
      R"((\d{4})-(([0][1-9])|([1][012]))-(([0][1-9])|([12][0-9])|([3][012])))");
  return !std::regex_match(input, pattern);
}
