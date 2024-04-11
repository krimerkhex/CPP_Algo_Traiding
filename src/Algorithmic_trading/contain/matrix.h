#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <exception>
#include <iostream>

namespace s21 {
class Matrix {
 public:
  Matrix();
  Matrix(const Matrix& other);
  Matrix(Matrix&& other);
  Matrix(const Matrix&& other);
  Matrix(const unsigned int row, const unsigned int col);
  Matrix(const unsigned int col);
  ~Matrix();

  Matrix& operator=(const Matrix& other);
  Matrix& operator=(Matrix&& other);
  Matrix& operator=(const Matrix&& other);
  Matrix& operator=(const double num);

  bool operator==(const Matrix& other) const;
  bool operator!=(const Matrix& other) const;

  void operator+=(const Matrix& other);
  void operator-=(const Matrix& other);
  void operator*=(const Matrix& other);
  void operator+=(const double num);
  void operator-=(const double num);
  void operator*=(const double num);

  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator+(const double num) const;
  Matrix operator-(const double num) const;
  Matrix operator*(const double num) const;

  void reSize(const unsigned int row, const unsigned int col);
  void print() const;

  double& operator()(const unsigned int row, const unsigned int col);
  double* operator()(const unsigned int row);

  unsigned int getRow() const;
  unsigned int getCol() const;
  unsigned int getSize() const;

  double getMaxVal();
  double getMinVal();

 private:
  unsigned int row_;
  unsigned int col_;
  double** matrix_;

  double** newData();
  void deleteData();
  void checkEquaLe(const Matrix& other) const;
  void checkMulti(const Matrix& other) const;
  void copy(const Matrix& other);
  void move(const Matrix& other);
};
}  // namespace s21

#endif  // MATRIX_H
